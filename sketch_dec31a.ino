#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#define Duman_sensor A0
#define BOT_TOKEN ""
#define CHAT_ID "338442260"
const char *ssid =  "Redmi";
const char *pass =  "halil123";
int BuzzerPin = D3;
int LED_yesil = D1;
int LED_kirmizi = D2;
int LED_mavi = D0;
int flamePin = D5;
float gasREAD = 0;
int flameDig = 0;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  pinMode(flamePin,INPUT);
  pinMode(BuzzerPin,OUTPUT);
  pinMode(LED_yesil,OUTPUT);
  pinMode(LED_kirmizi,OUTPUT);
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);
  attachInterrupt(digitalPinToInterrupt(2), interrupt_func, RISING);//Kesme modu
  
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
  analogWrite(BuzzerPin,0);
  gasREAD = analogRead(Duman_sensor);
  flameDig = digitalRead(flamePin);
  Serial.println(gasREAD);
  Serial.println(flameDig);
  delay(500);
  
  if(gasREAD >= 285  && flameDig == LOW){
    digitalWrite(LED_mavi,0); 
    digitalWrite(LED_kirmizi,1);
    digitalWrite(LED_yesil,0);
    Serial.println("WARNING ! GAS LEAK AND FLAME");
    Serial.println(gasREAD);
    Serial.println(flameDig);
    analogWrite(BuzzerPin,180);
    bot.sendMessage(CHAT_ID, "GAZ KAÇAĞI VE YANGIN ALARMI ! LÜTFEN 112 ARAYIN.", ""); //telegram message
    delay(500);
    }
  
   //Yesil led yaniyor
  
  else if(gasREAD < 285 && flameDig == LOW){ //LOW -> flame enabled
    digitalWrite(LED_mavi,0);
    digitalWrite(LED_kirmizi,1);
    digitalWrite(LED_yesil,0);
    Serial.println("WARNING ! FLAME");
    Serial.println(flameDig);
    analogWrite(BuzzerPin,180);
    bot.sendMessage(CHAT_ID, "YANGIN ALARMI ! LÜTFEN 112 ARAYIN.", ""); //telegram message
    delay(500);
  }
  else if(gasREAD >= 285 && flameDig == HIGH){ //LOW -> flame enabled
    digitalWrite(LED_mavi,0);
    digitalWrite(LED_kirmizi,1);
    digitalWrite(LED_yesil,0);
    Serial.println("WARNING ! GAS LEAK");
    Serial.println(flameDig);
    analogWrite(BuzzerPin,180);
    bot.sendMessage(CHAT_ID, "GAZ KAÇAĞI ALARMI ! LÜTFEN 112 ARAYIN.", ""); //telegram message
    delay(500);
  }
  else if(gasREAD > 700){
    digitalWrite(d4,1);//Kesme aktif hale gelir.
  }

  else{

  digitalWrite(LED_mavi,0);
  digitalWrite(LED_kirmizi,0);
  digitalWrite(LED_yesil,1);
  
  }
  }
  else{
    digitalWrite(LED_mavi,0);
    digitalWrite(LED_kirmizi,1);
    digitalWrite(LED_yesil,0);
    Serial.println("No connection !");
    delay(400);
    digitalWrite(LED_mavi,0);
    digitalWrite(LED_kirmizi,0);
    digitalWrite(LED_yesil,0);
    Serial.println("No connection !");
    delay(400);
  }
  
}
  void interrupt_func(){
    digitalWrite(LED_mavi,0);
    digitalWrite(LED_kirmizi,1);
    digitalWrite(LED_yesil,0);
    //Kirmizi led aktif
    bot.sendMessage(CHAT_ID, "Sensör arızası.Teknik servis ile iletişim kuruldu.", ""); //telegram service message
  }
