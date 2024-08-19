/**
   
   Tham khao JSON them: https://arduinojson.org/v6/doc/serialization/ 
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN D2
DHT dht(DHTPIN, DHTTYPE);

int pinD0 = D6;
int buzzer = D4;
int flameD0;


const char *SSID = "DoanTin";
const char *PASSWORD = "doantin135";
const char *URL = "http://192.168.1.6:5555/addData";

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

void setup() {
  
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] IN PROGRESS: %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print("");
  }
  Serial.println("");
  Serial.println("------WiFi connected------");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("--------------------------");
  Serial.println("BEGIN...");
  Serial.println("");
  Serial.println("");

  pinMode(pinD0, INPUT);
  pinMode(buzzer, OUTPUT);
   
}


void postJsonData() {
  Serial.print("Connecting to ");
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, URL)) {
      Serial.print("[HTTP] POST...\n");
      const int capacity = JSON_OBJECT_SIZE(3);
      StaticJsonDocument<capacity> doc;
      dht.begin();
      float t = dht.readTemperature();

      doc["id"] = "1";
      doc["device"] = "dht11";
      doc["temperature"] = t;


      char output[2048];
      serializeJson(doc, Serial);
      serializeJson(doc, output);


      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(output);
      Serial.println(httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
        char input[50];
        payload.toCharArray(input, 50);
        Serial.println("Begin parse json data ...");
        DynamicJsonDocument docResponse(2048);
        DeserializationError err = deserializeJson(docResponse, payload);
        if (err) {
          Serial.print(F("deserializeJson() failed with code "));
          Serial.println(err.c_str());
        }
        Serial.println("");
        Serial.println("---COMPLETED---");

      } else {
        Serial.printf("[HTTP] POST... failed; ERROR: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      Serial.println("Closing connection...");
      Serial.println("");
      Serial.println("-------------------------------------------------------------------------");
      Serial.print("- Temperature: ");
      Serial.println(t);
      Serial.println("-------------------------------------------------------------------------");
      Serial.println("");
    }
  }
}

void loop() {
  flameD0 = digitalRead(pinD0);
  if (flameD0 == 0) {
    digitalWrite(buzzer, HIGH);
    
    Serial.println("!---DETECTED---!");
    Serial.println("");
    postJsonData();
  } else {
    digitalWrite(buzzer, LOW);
    
  }
  delay(2000);
   
}


