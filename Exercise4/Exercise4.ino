#include <Arduino.h>
int led_pin = 13;
int switch_pin = 21;

void setup() {
  pinMode(led_pin,OUTPUT);
  pinMode(switch_pin,INPUT);
}

void loop() {
  if (digitalRead(switch_pin) == 0){
    for (int i = 0; i < 2; i++){
      digitalWrite(led_pin,HIGH);
      delay(50);
      digitalWrite(led_pin,LOW);
      delay(450);
    }
  } else {
    digitalWrite(led_pin,HIGH);
    delay(100);
    digitalWrite(led_pin,LOW);
    delay(900);
  }
  
}
