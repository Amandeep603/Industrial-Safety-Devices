#include <ESP8266WiFi.h>
#include <espnow.h>
#define buzzer 14
#define led1 12
#define led2 13
#define led3 15
#include<Servo.h>
Servo servo;
typedef struct struct_message {
  int lpg,t,h,board;
} struct_message;


struct_message myData;


void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Data from Board Number:");
  Serial.println(myData.board);
  Serial.print("LPG: ");
  Serial.println(myData.lpg);
  Serial.print("Temperature:");
  Serial.println(myData.t);
  Serial.print("Humidity:");
  Serial.println(myData.h);
}
 
void setup() {

Serial.begin(115200);
WiFi.mode(WIFI_STA);
servo.attach(5);
pinMode(buzzer,OUTPUT);
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(led3,OUTPUT);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if(myData.lpg==1){
    digitalWrite(led1,LOW);
  }
  else{
    digitalWrite(led1,LOW);
  }

  if(myData.t == 1){
  digitalWrite(led2,HIGH);
  digitalWrite(buzzer,HIGH);
  }
  else{
    digitalWrite(led2,LOW);
    digitalWrite(buzzer,LOW);
  }
  if(myData.h == 1){
  digitalWrite(led3,HIGH);
  digitalWrite(buzzer,HIGH);
  }
  else{
    digitalWrite(led3,LOW);
    digitalWrite(buzzer,LOW);
  }
  servo.write(0);
  delay(500);
  servo.write(90);
  servo.write(500);
  servo.write(180);
  delay(500);
}