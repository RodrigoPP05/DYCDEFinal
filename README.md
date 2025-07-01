# 🌱 Smart Air and Soil Monitor - Proyecto Final DYCDE 2025

Este proyecto tiene como objetivo principal monitorear y automatizar el ambiente en invernaderos y sistemas hidropónicos a pequeña escala mediante una solución IoT. Se integra con sensores para medir temperatura, humedad, presión, calidad del aire y humedad del suelo, así como actuadores como bombas y ventiladores, todo gestionado desde un ESP32.

Los datos se visualizan en tiempo real desde cualquier dispositivo móvil mediante una interfaz web creada en Node-RED, y se transmiten usando MQTT con notificaciones vía Telegram.

- Temperatura y humedad ambiental (BME680)
- Humedad del suelo (sensor capacitivo)
- Concentración de gases (MQ-135 / BME680)
- Nivel de agua (flotador)
- Tensión de alimentación (ZMPT101B)

Además, controla automáticamente una bomba, ventilador y lámpara LED mediante relés. Los datos son publicados vía MQTT y pueden visualizarse en dashboards interactivos de Node-RED. También se envían alertas al usuario mediante Telegram.

---

## ⚙️ Tecnologías y herramientas utilizadas

- **ESP32** como unidad principal de procesamiento.
- **Arduino IDE** para la programación del firmware.
- **Altium Designer** para el diseño esquemático y de la PCB.
- **Node-RED** como interfaz de control y visualización.
- **TinkerCAD** para el diseño 3D de la carcasa.
- **Mosquitto MQTT** para la comunicación de datos entre nodos.

---

## 📂 Contenido del repositorio

| Archivo/Carpeta                          | Descripción                                                                 |
|-----------------------------------------|-----------------------------------------------------------------------------|
| `Proyecto/`                              | Carpeta con archivos base del proyecto, como esquemáticos en Altium.       |
| `Presentación_de_Proyecto_DYCDE_2025.pdf` | Informe final en PDF que documenta todo el proyecto.                       |
| `Proyecto.DXF`                           | Modelo 2D del diseño de la carcasa/exportado desde TinkerCAD o Altium.     |
| `Proyecto.step`                          | Modelo 3D completo del sistema (incluye PCB y carcasa).                    |
| `Proyectofinal.ino`                      | Código para ESP32 (firmware compatible con Arduino IDE).                   |
| `README.md`                              | Archivo de descripción general del repositorio (este mismo documento).     |

---

## 📷 Capturas del sistema

- Interfaz en Node-RED con flujos de sensores, controladores y Telegram.
- Visualización de sensores (temperatura, humedad, suelo) desde el dashboard.
- Publicaciones MQTT en tiempo real desde Mosquitto.
- Diseño en 3D de la carcasa impresa y la PCB.

---

## 🔗 Cómo ejecutar

1. Abre `Proyectofinal.ino` en Arduino IDE y cárgalo al ESP32.
2. En Node-RED, importa el flujo (opcional).
3. Asegúrate de tener Mosquitto instalado como broker MQTT.
4. Configura WiFi, tópicos y tokens de Telegram en el código.

---

## 📥 Requisitos

- Arduino IDE con librerías: `Adafruit_BME680`, `DallasTemperature`, `OneWire`, `PubSubClient`, `UniversalTelegramBot`.
- Node-RED instalado en PC o Raspberry Pi.
- Mosquitto broker funcionando localmente o en la nube.
- Acceso a red WiFi.
- Cuenta de Telegram y creación de un bot con token.
- ESP32 DevKit v1 o equivalente
- Sensores: BME680, DS18B20, sensor de humedad capacitivo, ZMPT101B
- Módulo de relés (3 canales)
- Aplicación Telegram
---

## 📦 Descargas recomendadas

- [Arduino IDE](https://www.arduino.cc/en/software) – para cargar el firmware al ESP32
- [Altium Designer (versión de prueba)](https://www.altium.com/altium-designer/free-trial) – para editar el diseño de la PCB
- [Node-RED](https://nodered.org/docs/getting-started/) – para visualizar dashboards y controlar actuadores
- [Mosquitto MQTT](https://mosquitto.org/download/) – broker MQTT para pruebas locales o en la nube

## 📱 Visualización remota

El sistema puede monitorearse desde cualquier dispositivo móvil a través de:
- Interfaz web de Node-RED
- Alertas en Telegram con botones de control integrados

## 📧 Contacto

**Rodrigo Ponce Paz**  
Estudiante de Ingeniería en Electrónica  
Universidad Galileo - FISICC  
GitHub: [@RodrigoPP05](https://github.com/RodrigoPP05)

---

