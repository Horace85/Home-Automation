#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID  "xxxxx"
#define WIFI_PASS "xxxxxx"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "xxxxxx"
#define MQTT_PASS "xxxxx"

//Definimos los relays
#define relay D0 
#define relay1 D1 

//Definimos los botones
#define boton1 D6
#define boton2 D7

//Definimos los Feeds
#define FEED1 "/f/onoff2"
#define FEED2 "/f/onoff3"


//Setup del WiFi y del cliente MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Lee los datos de las queue 
Adafruit_MQTT_Subscribe onoff2 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME FEED1);
Adafruit_MQTT_Subscribe onoff3 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME FEED2);

//Publica datos en las queue
Adafruit_MQTT_Publish botonOn0ff2 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME FEED1);
Adafruit_MQTT_Publish botonOn0ff3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME FEED2);

/***************************************************************/

void setup()
{
  Serial.begin(9600);

  //Connect to WiFi
  Serial.print("\n\nConectando al Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");

//Suscribirse a los feed onoff 
  mqtt.subscribe(&onoff2);
  mqtt.subscribe(&onoff3);

  pinMode(relay, OUTPUT);
  pinMode(relay1, OUTPUT);
  digitalWrite(relay,HIGH);
  digitalWrite(relay1,HIGH);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
}

/***************************************************************/

void loop()
{
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(4000)))
  {

//Si llego hasta aca, el feed esta actualizado.
// Relay 1 ----------------------------------------------------------- 
    
    if (subscription == &onoff2)
    {
//Imprimer los valores en la consola
      Serial.print("onoff2: ");
      Serial.println((char*) onoff2.lastread);
      
//Si (if) el valor es "ON" prende la luz del relay 2
//de lo contrario(else) la apaga

      if (!strcmp((char*) onoff2.lastread, "ON"))
      {
        //Active low logic
          digitalWrite(relay,LOW);
      }
      else
      {
          digitalWrite(relay,HIGH);
      }
    }
    

    
// Relay 2 -----------------------------------------------------------
    
    if (subscription == &onoff3)
    {
//Imprime los valores en la consola
      Serial.print("onoff3: ");
      Serial.println((char*) onoff3.lastread);
      
//Si (if) el valor es "ON" prende la luz del relay 2
//de lo contrario(else) la apaga

      if (!strcmp((char*) onoff3.lastread, "ON"))
      {
        //Active low logic
          digitalWrite(relay1,LOW);
      }
      else
      {
          digitalWrite(relay1,HIGH);
      }
    }
  
  }


// Hace ping al servidor MQTT para mantener la conexion activa.

  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}
/***************************************************************/

void MQTT_connect() 
{
  int8_t ret;

  // No hacer nada si ya esta conectado
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Conectado a MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Reintentando conexion a MQTT en 4 segundos...");
       mqtt.disconnect();
       delay(4000);  // espera 4 segundos
       retries--;
       if (retries == 0) 
       {

         while (1);
       }
  }
  Serial.println("MQTT Conectado!");
}


