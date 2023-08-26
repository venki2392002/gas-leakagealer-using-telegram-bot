#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


#include <UniversalTelegramBot.h>

#define wifi_ssid "Your-wifi-ssid"
#define wifi_password "Your-wifi-password"
#define telegram_bot_token "your-telegram-bot-token"
#define telegram_chat_id "your-chat-id"
#define co_sensor_pin A0 // MQ-9 CO sensor connected to analog pin A0
//#define ch4_sensor_pin A1 // MQ-9 CH4 sensor connected to analog pin A1

WiFiClientSecure client;
UniversalTelegramBot bot(telegram_bot_token, client);

void setup() {
  Serial.begin(9600);
  pinMode(co_sensor_pin, INPUT);
  //pinMode(ch4_sensor_pin, INPUT);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
}

void loop() {
  int co_level = analogRead(co_sensor_pin); // read CO level from sensor
  //int ch4_level = analogRead(ch4_sensor_pin); // read CH4 level from sensor
  Serial.print("CO Level: ");
  Serial.print(co_level);
  //Serial.print(" | CH4 Level: ");
  //Serial.println(ch4_level);

  if (co_level > 100) { // threshold for dangerous CO or CH4 level
    sendAlert();
    delay(60000); // wait for 1 minute before sending another notification
  }

  delay(1000); // wait for 1 second before checking again
}

void sendAlert() {
  String message = "Dangerous gas levels detected!";
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Sending notification...");
    if (bot.sendMessage(telegram_chat_id, message)) {
      Serial.println("Telegram notification sent successfully!");
    } else {
      Serial.println("Failed to send Telegram notification!");
    }
  } else {
    Serial.println("WiFi not connected, cannot send notification.");
  }
}
