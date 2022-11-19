
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "badge.hpp"
#if defined(ESP8266)
#include <ESP8266WiFi.h>
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
int pump = D3;
#elif defined(ESP32)
#include <WiFi.h>
int pump = 27;
#endif

unsigned long bot_lasttime;
const unsigned long BOT_MTBS = 2000;


WiFiClientSecure secured_client;


UniversalTelegramBot bot(BOTtoken, secured_client);
int x = 0;

const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
  
  String answer;
  for (int i = 0; i < numNewMessages; i++){
    telegramMessage &msg = bot.messages[i];
    Serial.println("Received " + msg.text);
    if (msg.text == "/pumpON"){
      answer = "pump status is ON";
      digitalWrite(pump, LOW);
    }
    else if (msg.text == "/start"){
      answer = "My command:\n/pumpON\n/pumpOFF\n/getMoisture";
    }
    else if (msg.text == "/pumpOFF"){
      answer = "pump status is off";
      digitalWrite(pump, HIGH);
    }
    else if (msg.text == "/getMoisture"){
      answer = (String)analogRead(A0);
    }
    bot.sendMessage(msg.chat_id, answer, "Markdown");
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(pump, OUTPUT);
  digitalWrite(pump, HIGH);
  #if defined(ESP8266)
  secured_client.setTrustAnchors(&cert);
  #elif defined(ESP32)
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
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
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages){
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
  /*
  x = analogRead(0);
  bot.sendMessage(CHAT_ID, String(x), "");
  Serial.println(x);
  if (x > 520) {
    bot.sendMessage(CHAT_ID, "Pump on", "");
    delay(100);
    do {
      digitalWrite(pump, LOW);
      x = analogRead(0);
      delay(25000);
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
  */
}
