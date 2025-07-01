#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Pines
#define ONE_WIRE_BUS 4
#define PIN_RELE 15
#define PIN_RELE12 2
#define PIN_HUMEDAD_SUELO 36

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
Adafruit_BME680 bme;

// WiFi y MQTT
struct RedWiFi {
  const char* ssid;
  const char* password;
};

RedWiFi redesDisponibles[] = {
  {"Galaxy S10+eae2", "sbio0892"},
  {"TIGO-BB8F", "4G79ED802919"},
  {"CasaWifi", "claveCasa"},
  {"OtraRed", "claveOtra"}
};

const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);

// Telegram
const char* BOTtoken = "8119686924:AAEEjtBgANiSd2eGeN33igCYa2kWQp2HZMs";
const char* chat_id = "1776359286";
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);

// Estados
String ipESP32 = "";
bool alertaRiegoEnviada = false;

void mostrarBotones() {
  String keyboardJson =
    "[[{\"text\":\"📊 Estado\",\"callback_data\":\"estado\"},"
    "{\"text\":\"🔁 Reiniciar\",\"callback_data\":\"reiniciar\"}],"
    "[{\"text\":\"#3 ⚡\",\"callback_data\":\"encender_rele\"},"
    "{\"text\":\"#3 ❌\",\"callback_data\":\"apagar_rele\"}],"
    "[{\"text\":\"BL💡\",\"callback_data\":\"encender_rele12\"},"
    "{\"text\":\"BL ❌\",\"callback_data\":\"apagar_rele12\"}],"
    "[{\"text\":\"🌐 Ver Panel\",\"url\":\"http://" + ipESP32 + ":1880/ui\"}]]";

  bot.sendMessageWithInlineKeyboard(chat_id, "¿Qué deseas hacer?", "", keyboardJson);
}

void setup_wifi() {
  delay(100);
  Serial.println("🔍 Escaneando redes WiFi...");

  int cantidad = WiFi.scanNetworks();
  int mejorRSSI = -1000;
  const char* mejorSSID = nullptr;
  const char* mejorPass = nullptr;

  for (int i = 0; i < cantidad; i++) {
    String ssidDetectada = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);

    for (RedWiFi red : redesDisponibles) {
      if (ssidDetectada == red.ssid && rssi > mejorRSSI) {
        mejorRSSI = rssi;
        mejorSSID = red.ssid;
        mejorPass = red.password;
      }
    }
  }

  if (mejorSSID != nullptr) {
    Serial.printf("📶 Conectando a: %s (RSSI: %d)\n", mejorSSID, mejorRSSI);
    WiFi.begin(mejorSSID, mejorPass);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500); Serial.print(".");
    }

    Serial.println("\n✅ Conectado a WiFi");
    ipESP32 = WiFi.localIP().toString();
    Serial.println("IP: " + ipESP32);

    secured_client.setInsecure();
    bot.sendMessage(chat_id, "☁️ ESP32 conectado a WiFi: " + String(mejorSSID), "");
    bot.sendMessage(chat_id, "🌐 IP local del ESP32: " + ipESP32, "");

    mostrarBotones();
    delay(1000);
    bot.getUpdates(bot.last_message_received + 1);
  } else {
    Serial.println("❌ Ninguna red conocida encontrada.");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32SensorNode")) {
      Serial.println(" conectado.");
      client.subscribe("GalileoUI/rele");
      client.subscribe("GalileoUI/rele12");
    } else {
      Serial.print(" falló, rc="); Serial.println(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) message += (char)payload[i];

  if (String(topic) == "GalileoUI/rele")
    digitalWrite(PIN_RELE, message == "1" ? LOW : HIGH);

  if (String(topic) == "GalileoUI/rele12")
    digitalWrite(PIN_RELE12, message == "1" ? LOW : HIGH);
}

void enviarEstado(String id, float tempDS, float hum, float pres, float gas, float humSuelo) {
  String respuesta = "📡 Estado actual:\n";
  respuesta += "🌡️ DS18B20: " + String(tempDS, 2) + " °C\n";
  respuesta += "💧 Humedad: " + String(hum, 2) + " %\n";
  respuesta += "🧭 Presión: " + String(pres, 2) + " hPa\n";
  respuesta += "🫧 Gas: " + String(gas, 2) + " KΩ\n";
  respuesta += "🌱 Suelo: " + String(humSuelo, 1) + " %";
  bot.sendMessage(id, respuesta, "");
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELE, OUTPUT); digitalWrite(PIN_RELE, HIGH);
  pinMode(PIN_RELE12, OUTPUT); digitalWrite(PIN_RELE12, HIGH);
  pinMode(PIN_HUMEDAD_SUELO, INPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  ds18b20.begin();

  if (!bme.begin()) {
    bot.sendMessage(chat_id, "❌ No se detectó el BME680.", "");
    while (1);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setGasHeater(320, 150);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  ds18b20.requestTemperatures();
  float tempDS = ds18b20.getTempCByIndex(0);
  if (tempDS == 85.0 || tempDS == DEVICE_DISCONNECTED_C)
    bot.sendMessage(chat_id, "⚠️ DS18B20 no responde", "");

  if (!bme.performReading()) return;

  float hum = bme.humidity;
  float pres = bme.pressure / 100.0;
  float gas = bme.gas_resistance / 1000.0;

  int valorCrudo = analogRead(PIN_HUMEDAD_SUELO);
  float humedadSuelo = map(valorCrudo, 2996, 1180, 0, 100);
  humedadSuelo = constrain(humedadSuelo, 0, 100);
  Serial.printf("🌱 Suelo crudo: %d | Humedad: %.1f %%\n", valorCrudo, humedadSuelo);

  // Activación automática del riego
//  if (humedadSuelo < 50.0) {
//    digitalWrite(PIN_RELE12, LOW);
//    if (!alertaRiegoEnviada) {
//      bot.sendMessage(chat_id, "🚨 Humedad baja: " + String(humedadSuelo, 1) + " %. Activando bomba y LED.", "");
//      alertaRiegoEnviada = true;
//    }
//  } else {
//    digitalWrite(PIN_RELE12, HIGH);
//    if (alertaRiegoEnviada) {
//      bot.sendMessage(chat_id, "✅ Humedad recuperada: " + String(humedadSuelo, 1) + " %. Bomba y LED apagados.", "");
//      alertaRiegoEnviada = false;
//    }
//  }

  // MQTT
  client.publish("GalileoUI/temp_DS18", (String(tempDS, 2) + " °C").c_str());
  client.publish("GalileoUI/humedad", (String(hum, 2) + " %").c_str());
  client.publish("GalileoUI/presion", (String(pres, 2) + " hPa").c_str());
  client.publish("GalileoUI/gas", (String(gas, 2) + " KΩ").c_str());
  client.publish("GalileoUI/humedad_suelo", (String(humedadSuelo, 1) + " %").c_str());

  // Telegram comandos
  int numMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numMessages) {
    for (int i = 0; i < numMessages; i++) {
      String tipo = bot.messages[i].type;
      String texto = bot.messages[i].text;
      String chat = bot.messages[i].chat_id;
      String query_id = bot.messages[i].query_id;

      if (tipo == "message") {
        if (texto == "/estado" || texto == "📊 Estado")
          enviarEstado(chat, tempDS, hum, pres, gas, humedadSuelo);
        else if (texto == "/reiniciar" || texto == "🔁 Reiniciar") {
          bot.sendMessage(chat, "♻️ Reiniciando ESP32...", "");
          delay(1000); ESP.restart();
        }
      }

      if (tipo == "callback_query") {
        if (texto == "estado") {
          enviarEstado(chat, tempDS, hum, pres, gas, humedadSuelo);
          bot.answerCallbackQuery(query_id, "📊 Estado enviado");
        } else if (texto == "reiniciar") {
          bot.answerCallbackQuery(query_id, "♻️ Reiniciando...");
          bot.sendMessage(chat, "🔄 Reiniciando ESP32...", "");
          delay(1000); ESP.restart();
        } else if (texto == "encender_rele") {
          digitalWrite(PIN_RELE, LOW);
          bot.answerCallbackQuery(query_id, "⚡ Relé encendido");
        } else if (texto == "apagar_rele") {
          digitalWrite(PIN_RELE, HIGH);
          bot.answerCallbackQuery(query_id, "❌ Relé apagado");
        } else if (texto == "encender_rele12") {
          digitalWrite(PIN_RELE12, LOW);
          bot.answerCallbackQuery(query_id, "⚡ Relé12 encendido");
        } else if (texto == "apagar_rele12") {
          digitalWrite(PIN_RELE12, HIGH);
          bot.answerCallbackQuery(query_id, "❌ Relé12 apagado");
        }
      }
    }
    numMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  delay(2000);
}
