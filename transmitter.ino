#include <ESP8266WiFi.h>
#include <espnow.h>
#define lpgPin 4
#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT11

#include<Servo.h>
DHT dht(DHTPIN, DHTTYPE);
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

Servo servo;
typedef struct struct_message {
  int lpg,t,h,board=1;
} struct_message;

struct_message myData;



void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  Serial.begin(115200);
  
  servo.attach(5);
 pinMode(lpgPin,INPUT);
  dht.begin();
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  Serial.print("Humidity:");
  Serial.println(h);
   Serial.print("Temperature:");
  Serial.println(t);
if(analogRead(lpgPin)>1000){
  myData.lpg = 1;
}
else{
  myData.lpg = 0;
}


if(h>80){
  myData.h =1;
}
else{
  myData.h =0;
}


if(t>30){
  myData.t =1;
}
else{
  myData.t =0;

}
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
delay(100);
  servo.write(0);
  delay(500);
  servo.write(90);
  servo.write(500);
  servo.write(180);
  delay(500);
}