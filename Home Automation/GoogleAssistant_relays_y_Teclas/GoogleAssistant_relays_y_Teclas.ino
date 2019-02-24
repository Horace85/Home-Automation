/*
Control de Relays con MQTT y llaves de punto fisicas, Para automatizacion del hogar.

Contributors:
   -diegohce

Fecha: 23/02/2019
Autor: Horacio Rico
*/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID  "SSID"
#define WIFI_PASS "PASSWORD"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "USUARIO"
#define MQTT_PASS "KEY"

//Definimos los Relays
#define Rel1 D0 
#define Rel2 D1 

//Definimos los pines de los botones:
#define Tecla1 D2
#define Tecla2 D3


//Definimos las variables de los los Feeds:
#define FEED1 "/f/onoff0"  //Editar con el feed que corresponda
#define FEED2 "/f/onoff1"  //Editar con el feed que corresponda

//Setup del WiFi y del cliente MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Lee los datos de las queue 
Adafruit_MQTT_Subscribe onoff2 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME FEED1);
Adafruit_MQTT_Subscribe onoff3 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME FEED2);

//Publica datos en las queue
Adafruit_MQTT_Publish botonOn0ff2 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME FEED1);
Adafruit_MQTT_Publish botonOn0ff3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME FEED2);

//Definimos tiempo y las variables volatiles
unsigned long tiempo;
volatile int EstadoT1 = HIGH;
volatile int EstadoT2 = HIGH;
int tecla1 = digitalRead(Tecla1);
int tecla2 = digitalRead(Tecla2);


//Configuramos los callbacks de los feeds y la logica de la accion.
void onoff2callback(char *onoff2, uint16_t len) {
   if ((char)*onoff2 == '1' && EstadoT1 == HIGH && tecla1 == HIGH ) {
    EstadoT1 = LOW;
  } 
  else if ((char)*onoff2 == '1' && EstadoT1 == HIGH && tecla1 == LOW ) {
    EstadoT1 = LOW;
  }
  else if ((char)*onoff2 == '0' && EstadoT1 == LOW && tecla1 == HIGH ) {
    EstadoT1 = HIGH;
  }
  else if ((char)*onoff2 == '0' && EstadoT1 == LOW && tecla1 == LOW ) {
    EstadoT1 = HIGH;
  }

  digitalWrite(Rel1, EstadoT1);
    
  Serial.print("Llego un msj a ono/ff1, el estado es: ");
  Serial.println(onoff2);
  Serial.print("Var Estado en callback es: ");
  Serial.println(EstadoT1);
}

void onoff3callback(char *onoff3, uint16_t len) {
   if ((char)*onoff3 == '1' && EstadoT2 == HIGH && tecla2 == HIGH ) {
    EstadoT2 = LOW;
  } 
  else if ((char)*onoff3 == '1' && EstadoT2 == HIGH && tecla2 == LOW ) {
    EstadoT2 = LOW;
  }
  else if ((char)*onoff3 == '0' && EstadoT2 == LOW && tecla2 == HIGH ) {
    EstadoT2 = HIGH;
  }
  else if ((char)*onoff3 == '0' && EstadoT2 == LOW && tecla2 == LOW ) {
    EstadoT2 = HIGH;
  }

  digitalWrite(Rel2, EstadoT2);
    
  Serial.print("Llego un msj a onoff3, el estado es: ");
  Serial.println(onoff3);
  Serial.print("Var Estado en callback2 es: ");
  Serial.println(EstadoT2);
}

//Configuramos la funcion de cambio, para cuando detecte una alteracion en el pin de la tecla
void CambioT1() {
  if(millis() - tiempo > 150){
   EstadoT1 = !EstadoT1 ;
   tiempo = millis();
   Serial.print("cambio detectado ");
   Serial.println(EstadoT1);
   }

   digitalWrite(Rel1, EstadoT1);
}

void CambioT2() {
  if(millis() - tiempo > 150){
   EstadoT2 = !EstadoT2 ;
   tiempo = millis();
   Serial.print("cambio detectado ");
   Serial.println(EstadoT2);
   }

   digitalWrite(Rel2, EstadoT2);
}


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
  
//Reles como Salida y que inicien apagados:
  pinMode(Rel1, OUTPUT);
  pinMode(Rel2, OUTPUT);
  digitalWrite(Rel1,HIGH);
  digitalWrite(Rel2,HIGH);

//Teclas en modo entrada con pullup, tambien definimos un valor para la var: tiempo  
  pinMode(Tecla1, INPUT_PULLUP);
  pinMode(Tecla2, INPUT_PULLUP);
  tiempo = 0;
  onoff2.setCallback(onoff2callback);
  onoff3.setCallback(onoff3callback);

//definimos las interrupciones
  attachInterrupt(digitalPinToInterrupt(Tecla1), CambioT1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Tecla2), CambioT2, CHANGE);
  
}

/***************************************************************/

void loop()
{
  MQTT_connect();

//Tiempo en el que va a procesar los datos de los callbacks.
  mqtt.processPackets(5000);

 //digitalWrite(Rel1, Estado);
 Serial.print("el valor de EstadoT1 es: ");
 Serial.println(EstadoT1);
 Serial.print("el valor de EstadoT2 es: ");
 Serial.println(EstadoT2);
  
if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}

/***************************************************************/



