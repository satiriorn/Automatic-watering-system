#include <ESP8266WiFi.h>
#include <ESP.h>
#include "driver/gpio.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "badge.hpp"
 
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
int pump = D3;
UniversalTelegramBot bot(BOTtoken, secured_client);
int x = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(pump, OUTPUT);
  secured_client.setTrustAnchors(&cert);
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  x = analogRead(0);
  Serial.println(x);
  if (x > 520) {
    bot.sendMessage(CHAT_ID, String(x) + "Pump on", "");
    digitalWrite(pump, HIGH);
    do {
      digitalWrite(pump, HIGH);
      x = analogRead(0);
      delay(5000);
    }
    while (x > 430);
    Serial.println("DEEP SLEEEP");
    digitalWrite(pump, LOW);
    //
    ESP.deepSleep(3600000000UL);
  }
  else {
    Serial.println("DEEP SLEEEP");
    bot.sendMessage(CHAT_ID, String(x), "");
    gpio_pin_wakeup_disable();
    //ESP.gpio_hold_en(pump);
    digitalWrite(pump, LOW);
    ESP.deepSleep(3600000000UL);
  }
}
