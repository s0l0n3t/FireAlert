#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#define SMOKE_SENSOR_PIN A0
#define BOT_TOKEN "CREATED BOT TOKEN" 
#define CHAT_ID "338442260"
const char *SSID =  "WIFI_SSID";
const char *WIFI_PASSWORD =  "WIFI_PASSWORD";
int const BUZZER_PIN = D3;
int const LED_GREEN = D1;
int const LED_RED = D2;
int const LED_BLUE = D0;
int const FLAME_SENSOR_PIN = D5;
float smokeVar = 0;
int flameVar = 0;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);


void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID,WIFI_PASSWORD);
  pinMode(FLAME_SENSOR_PIN,INPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);
  attachInterrupt(digitalPinToInterrupt(2), interrupt_func, RISING);//Kesme modu
  
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
  analogWrite(BUZZER_PIN,0);
  smokeVar = analogRead(SMOKE_SENSOR_PIN);
  flameVar = digitalRead(FLAME_SENSOR_PIN);
  Serial.println(smokeVar);
  Serial.println(flameVar);
  delay(500);
  
  if(smokeVar >= 285  && flameVar == LOW){
    digitalWrite(LED_BLUE,0); 
    digitalWrite(LED_RED,1);
    digitalWrite(LED_GREEN,0);
    Serial.println("WARNING ! GAS LEAK AND FLAME");
    Serial.println(smokeVar);
    Serial.println(flameVar);
    analogWrite(BUZZER_PIN,180);
    bot.sendMessage(CHAT_ID, "GAZ KAÇAĞI VE YANGIN ALARMI ! LÜTFEN 112 ARAYIN.", ""); //telegram message
    delay(500);
    }
  
   //Yesil led yaniyor
  
  else if(smokeVar < 285 && flameVar == LOW){ //LOW -> flame enabled
    digitalWrite(LED_BLUE,0);
    digitalWrite(LED_RED,1);
    digitalWrite(LED_GREEN,0);
    Serial.println("WARNING ! FLAME");
    Serial.println(flameVar);
    analogWrite(BUZZER_PIN,180);
    bot.sendMessage(CHAT_ID, "YANGIN ALARMI ! LÜTFEN 112 ARAYIN.", ""); //telegram message
    delay(500);
  }
  else if(smokeVar >= 285 && flameVar == HIGH){ //LOW -> flame enabled
    digitalWrite(LED_BLUE,0);
    digitalWrite(LED_RED,1);
    digitalWrite(LED_GREEN,0);
    Serial.println("WARNING ! GAS LEAK");
    Serial.println(flameVar);
    analogWrite(BUZZER_PIN,180);
    bot.sendMessage(CHAT_ID, "GAZ KAÇAĞI ALARMI ! LÜTFEN 112 ARAYIN.", ""); //telegram message
    delay(500);
  }
  else if(smokeVar > 700){
    digitalWrite(d4,1);//Kesme aktif hale gelir.
  }

  else{

  digitalWrite(LED_BLUE,0);
  digitalWrite(LED_RED,0);
  digitalWrite(LED_GREEN,1);
  
  }
  }
  else{
    digitalWrite(LED_BLUE,0);
    digitalWrite(LED_RED,1);
    digitalWrite(LED_GREEN,0);
    Serial.println("No connection !");
    delay(400);
    digitalWrite(LED_BLUE,0);
    digitalWrite(LED_RED,0);
    digitalWrite(LED_GREEN,0);
    Serial.println("No connection !");
    delay(400);
  }
  
}
  void interrupt_func(){
    digitalWrite(LED_BLUE,0);
    digitalWrite(LED_RED,1);
    digitalWrite(LED_GREEN,0);
    //Kirmizi led aktif
    bot.sendMessage(CHAT_ID, "Sensör arızası.Teknik servis ile iletişim kuruldu.", ""); //telegram service message
  }
