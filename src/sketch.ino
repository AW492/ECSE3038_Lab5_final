#include <LiquidCrystal.h>
#include <Arduino.h>
#include <WiFi.h>
#include "env.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Define LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(4, 5, 18, 19, 21, 22);

void setup() {

  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  lcd.begin(16, 2);  // Initialize LCD (16x2)
  lcd.setCursor(0, 0);
  lcd.print("Hello, ESP32!");
  
  if (IS_WOKWI) 
  WiFi.begin(SSID, PASS, CHANNEL);
  else 
  WiFi.begin(SSID, PASS);

  while(WiFi.status() != WL_CONNECTED){

    delay(500);
    Serial.print(".");
  }
  Serial.print("Wifi connect. IP address is: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  if (WiFi.status()==WL_CONNECTED){

    HTTPClient http;
    http.begin(ENDPOINT);
    http.addHeader("api-key",(API_KEY));
    int responseCode = http.GET();

    Serial.print("HTTP Response: ");
    Serial.println(responseCode);

    if (responseCode == HTTP_CODE_OK){
      String answer = http.getString();
      Serial.print("HTTP Response: ");
      Serial.println(answer);

      JsonDocument object;
      DeserializationError error = deserializeJson(object, answer);

      if(error){
        Serial.print("Deserialization failed: ");
        Serial.println(error.c_str());
      }else {
        const char* line_1 = object["line_1"];
        const char* line_2 = object["line_2"];

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(line_1);
        lcd.setCursor(0,1);
        lcd.print(line_2);

        Serial.print("Line 1: ");
        Serial.println(line_1);
        Serial.print("Line 2: ");
        Serial.println(line_2);
      }
    } else {
      Serial.print("HTTP Request Failed: ");
      Serial.println(responseCode);
    }

    http.end();
  } else {
    Serial.println("Wifi Connection Lost");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wifi Lost");
  }
  delay(1000);

}
