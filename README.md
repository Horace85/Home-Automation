# Home Automation - Control de Relays con Google Assistant y MQTT

Este proyecto permite la automatización del hogar mediante el control de relays utilizando un NodeMCU ESP8266, Google Assistant y MQTT. Se integra con Adafruit IO para la comunicación con la nube y permite el control de dispositivos eléctricos tanto a través de comandos remotos como con interruptores físicos.

## 📷 Diagrama de Conexión

![Diagrama de Conexión](Conexiones.PNG)

## ⚙️ Características
- ✅ Control de luces mediante **Google Assistant** y MQTT.
- ✅ Integración con **Adafruit IO** para la comunicación.
- ✅ **Interruptores físicos** para control manual.
- ✅ **Feedback de estado** de los relays en tiempo real.
- ✅ Reintentos automáticos de conexión a MQTT en caso de fallo.

## 🚀 Instalación y Configuración

### 🛠️ Requisitos
- **NodeMCU ESP8266**
- **Módulo de relays**
- **Fuente de alimentación Hi-Link 220V a 5V**
- **Adafruit IO account**
- **Google Assistant configurado**
- **Bibliotecas necesarias:**
  - `ESP8266WiFi.h`
  - `Adafruit_MQTT.h`
  - `Adafruit_MQTT_Client.h`

### 📌 Configuración del Código
1. Clona este repositorio:
   ```bash
   git clone https://github.com/Horace85/Home-Automation.git
   ```
2. Modifica las credenciales de WiFi en el código:
   ```cpp
   #define WIFI_SSID  "TU_SSID"
   #define WIFI_PASS "TU_PASSWORD"
   ```
3. Configura los datos de MQTT en el código:
   ```cpp
   #define MQTT_SERV "io.adafruit.com"
   #define MQTT_NAME "TU_USUARIO"
   #define MQTT_PASS "TU_KEY"
   ```
4. Sube el código al ESP8266 usando **Arduino IDE** o **PlatformIO**.

## 📡 Uso del Proyecto
- 🔹 **Encender/Apagar luces desde Google Assistant** usando comandos de voz.
- 🔹 **Encender/Apagar luces desde la app de Adafruit IO.**
- 🔹 **Control manual usando los interruptores físicos.**
- 🔹 **Sincronización automática entre los estados físicos y remotos.**

## 🛠 Troubleshooting
- ⚠️ **No se conecta a WiFi:** Verifica las credenciales en el código.
- ⚠️ **No responde a comandos MQTT:** Asegúrate de que la cuenta de Adafruit IO esté configurada correctamente.
- ⚠️ **Los relays no funcionan:** Revisa las conexiones eléctricas y la alimentación del circuito.

---

📧 Para consultas, contacta con **Horacio Rico** en GitHub.
