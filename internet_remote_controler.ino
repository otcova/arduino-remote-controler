#include "LowPower.h"

#define SWITCH_PIN 5
#define TRANSISTOR_BASE_PIN 2
#define BUZZER_PIN 9
#define BUZZER_GND_PIN 7

//#define PRINTLN(x) Serial.println
//#define PRINT(x) Serial.print
#define PRINTLN(x)
#define PRINT(x)

char previousData = 0;

void setup() {
  pinMode(SWITCH_PIN, OUTPUT);
  
  pinMode(TRANSISTOR_BASE_PIN, OUTPUT);
  digitalWrite(TRANSISTOR_BASE_PIN, HIGH);
  
  pinMode(BUZZER_GND_PIN, OUTPUT);
  digitalWrite(BUZZER_GND_PIN, LOW);
  
  analogWrite(BUZZER_PIN, 0);

  //Serial.begin(9600);
  //while (!Serial);
  PRINTLN(":)");
}

void loop() {
  char data = fetchGet("internet-remote-controler.herokuapp.com", "/state");
  if (data != 0 && previousData != data) {
    if (previousData != 0) triggerSwitch();
    previousData = data;
  }
  sleep();
}

void triggerSwitch() {
  warning_beep();
  digitalWrite(SWITCH_PIN, HIGH);
  delay(500);
  digitalWrite(SWITCH_PIN, LOW);
}

void warning_beep() {
  for (int i = 0; i < 10; ++i) {
    analogWrite(BUZZER_PIN, 10);
    delay(100);
    analogWrite(BUZZER_PIN, 0);
    delay(30);
  }
}
