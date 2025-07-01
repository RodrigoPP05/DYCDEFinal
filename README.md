# DYCDEFinal
Proyecto Final 

# 🌱 Smart Air and Soil Monitor – Proyecto IoT para Agricultura Urbana

Este proyecto desarrollado en el curso Diseño y Construcción de Dispositivos Electrónicos (DYCDE) consiste en un sistema IoT completo capaz de monitorear y controlar variables ambientales en cultivos urbanos, invernaderos o sistemas hidropónicos a pequeña escala. El sistema está basado en un ESP32 y sensores que permiten medir:

- Temperatura y humedad ambiental (BME680)
- Humedad del suelo (sensor capacitivo)
- Bomba de agua (Relé)
- Tensión de alimentación (ZMPT101B)

Además, controla automáticamente una bomba, ventilador y lámpara LED mediante relés. Los datos son publicados vía MQTT y pueden visualizarse en dashboards interactivos de Node-RED. También se envían alertas al usuario mediante Telegram.

## 📁 Contenido del repositorio

- `/hardware/`: Archivos del esquemático y PCB en Altium Designer.
- `/firmware/`: Código fuente para ESP32 en Arduino IDE.
- `/node-red/`: Flujo de Node-RED exportado (.json).
- `/documentacion/`: Informe técnico y resultados del proyecto.
- `/imagenes/`: Capturas del dashboard, esquemático y carcasa 3D.

## 🔧 Requisitos

- ESP32 DevKit v1 o equivalente
- Sensores: BME680, DS18B20, sensor de humedad capacitivo, ZMPT101B
- Módulo de relés (3 canales)
- Conexión Wi-Fi
- Aplicación Telegram

## 📦 Descargas recomendadas

- [Arduino IDE](https://www.arduino.cc/en/software) – para cargar el firmware al ESP32
- [Altium Designer (versión de prueba)](https://www.altium.com/altium-designer/free-trial) – para editar el diseño de la PCB
- [Node-RED](https://nodered.org/docs/getting-started/) – para visualizar dashboards y controlar actuadores
- [Mosquitto MQTT](https://mosquitto.org/download/) – broker MQTT para pruebas locales o en la nube

## 📱 Visualización remota

El sistema puede monitorearse desde cualquier dispositivo móvil a través de:
- Interfaz web de Node-RED
- Alertas en Telegram con botones de control integrados

## 💡 Licencia

Este proyecto es de código abierto bajo la licencia MIT. Si lo usas o adaptas, ¡menciónanos!

