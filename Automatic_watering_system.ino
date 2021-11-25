#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
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
  digitalWrite(pump, HIGH);
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
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop() {
  x = analogRead(0);
  bot.sendMessage(CHAT_ID, String(x), "");
  Serial.println(x);
  if (x > 520) {
    bot.sendMessage(CHAT_ID, "Pump on", "");
    delay(100);
    do {
      digitalWrite(pump, LOW);
      x = analogRead(0);
      delay(5000);
    }
    while (x > 430);
    Serial.println("DEEP SLEEEP");
    bot.sendMessage(CHAT_ID, "Pump off", "");
    ESP.deepSleep(3600000000UL);
  }
  else {
    Serial.println("DEEP SLEEEP");
    ESP.deepSleep(3600000000UL);
  }
}
