#include <Arduino.h>

#include "security.h"
/* Add below information in the security.h file */
// #define WIFI_SSID         "xxxxx"
// #define WIFI_PASS         "xxxxx"
// #define BLYNK_TEMPLATE_ID "xxxxx"
// #define BLYNK_DEVICE_NAME "xxxxx"
// #define BLYNK_AUTH_TOKEN "xxxxxxxxxxxx"

#if ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>
#elif ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#else
#error "Unsupported ARDUINO architecture"
#endif

#define BLYNK_PRINT Serial

#define RELEY1 0
#define RELEY2 1
#define RELEY3 2
#define RELEY4 3

typedef struct {
  uint32 lunes:1;
  uint32 martes:1;
  uint32 miercoles:1;
  uint32 jueves:1;
  uint32 viernes:1;
  uint32 sabado:1;
  uint32 domingo:1;
  uint32 hora:5;
  uint32 min:6;
  uint32 duration:4;
} info_t;

info_t inf[4];
info_t temp_inf;
uint8 temp_reley;

// Send the current values to the app
// send the values for reley1, reley are mutal exclusive
BLYNK_CONNECTED () 
{
  Blynk.virtualWrite(V10,true);
  Blynk.virtualWrite(V11,false);
  Blynk.virtualWrite(V12,false);
  Blynk.virtualWrite(V13,false);  
  Blynk.virtualWrite(V0,inf[0].lunes);
  Blynk.virtualWrite(V1,inf[0].martes);
  Blynk.virtualWrite(V2,inf[0].miercoles);
  Blynk.virtualWrite(V3,inf[0].jueves);
  Blynk.virtualWrite(V4,inf[0].viernes);
  Blynk.virtualWrite(V5,inf[0].sabado);
  Blynk.virtualWrite(V6,inf[0].domingo);
  Blynk.virtualWrite(V7,inf[0].hora);
  Blynk.virtualWrite(V8,inf[0].min);
  Blynk.virtualWrite(V9,inf[0].duration);

}

BLYNK_WRITE_DEFAULT()
{
  uint8_t pin = request.pin;
  switch (pin)
  {
  case V18:   
    // "SAVE" pin has been pressed, save the values to the 
    // corresponding reley
    Blynk.syncAll();
    inf[temp_reley].lunes = temp_inf.lunes;
    inf[temp_reley].martes = temp_inf.martes;
    inf[temp_reley].miercoles = temp_inf.miercoles;
    inf[temp_reley].jueves = temp_inf.jueves;
    inf[temp_reley].viernes = temp_inf.viernes;
    inf[temp_reley].sabado = temp_inf.sabado;
    inf[temp_reley].domingo = temp_inf.domingo;
    inf[temp_reley].hora = temp_inf.hora;
    inf[temp_reley].min = temp_inf.min;
    inf[temp_reley].duration = temp_inf.duration;
    Serial.printf("\nConfiguration for relay %u is\n",(temp_reley + 1));
    Serial.printf("Lunes = %u ",temp_inf.lunes);
    Serial.printf("martes = %u ",temp_inf.martes);
    Serial.printf("miercoles = %u ",temp_inf.miercoles);
    Serial.printf("jueves = %u ",temp_inf.jueves);
    Serial.printf("viernes = %u ",temp_inf.viernes);
    Serial.printf("sabado = %u ",temp_inf.sabado);
    Serial.printf("domingo = %u ",temp_inf.domingo);
    Serial.printf("\n%2u:%2u for %u minutes\n",temp_inf.hora,temp_inf.min,temp_inf.duration);
    Blynk.virtualWrite(V18,false);
    break;
  default:
    break;
  }
}

// Select reley1, disable 2,3,4
BLYNK_WRITE(V10)
{
  Blynk.virtualWrite(V11,false);
  Blynk.virtualWrite(V12,false);
  Blynk.virtualWrite(V13,false);
  temp_reley = RELEY1;
}

// Select reley2, disable 1,3,4
BLYNK_WRITE(V11)
{
  Blynk.virtualWrite(V10,false);
  Blynk.virtualWrite(V12,false);
  Blynk.virtualWrite(V13,false);
  temp_reley = RELEY2;
}

// Select reley3, disable 1,2,4
BLYNK_WRITE(V12)
{
  Blynk.virtualWrite(V10,false);
  Blynk.virtualWrite(V11,false);
  Blynk.virtualWrite(V13,false);
  temp_reley = RELEY3;
}

// Select reley4, disable 1,2,3
BLYNK_WRITE(V13)
{
  Blynk.virtualWrite(V10,false);
  Blynk.virtualWrite(V11,false);
  Blynk.virtualWrite(V12,false);
  temp_reley = RELEY4;
}

BLYNK_WRITE(V0)
{
  temp_inf.lunes = param.asInt();
  //Serial.printf("Valor en Lunes = %d",pinValue);
  //digitalWrite(LED_BUILTIN, pinValue);
}

BLYNK_WRITE(V1)
{
  temp_inf.martes = param.asInt();
//  Serial.printf("Valor en Martes = %d",pinValue);
}

BLYNK_WRITE(V2)
{
  temp_inf.miercoles = param.asInt();
//  Serial.printf("Valor en Miercoles = %d",pinValue);
}

BLYNK_WRITE(V3)
{
//  Serial.printf("Valor en Jueves = %d",pinValue);
}

BLYNK_WRITE(V4)
{
  temp_inf.viernes = param.asInt();
//  Serial.printf("Valor en Viernes = %d",pinValue);
}
BLYNK_WRITE(V5)
{
  temp_inf.sabado = param.asInt();
//  Serial.printf("Valor en Sabado = %d",pinValue);
}
BLYNK_WRITE(V6)
{
  temp_inf.domingo = param.asInt();
//  Serial.printf("Valor en Domingo = %d",pinValue);
}

BLYNK_WRITE(V7)
{
  temp_inf.hora = param.asInt();
  //Serial.printf("Valor en Horas = %d",pinValue);
}

BLYNK_WRITE(V8)
{
  temp_inf.min = param.asInt();
  //Serial.printf("Valor en Minutos = %d",pinValue);
}

BLYNK_WRITE(V9)
{
  temp_inf.duration = param.asInt();
  // Serial.printf("Valor en Duracion = %d",pinValue);
}

void setup()
{
  Serial.begin (115200);
  delay(500);
  Serial.println("Iniciando y conectando");
  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  // Post Blynk inits.
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop()
{
  // Loop to update Blynk state.
  Blynk.run();
  Serial.print("-");
  delay(1000);
}