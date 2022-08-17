#include "LowPower.h"

#define LED_PIN 5
#define TRANSISTOR_BASE_PIN 12
#define BUZZER_PIN 9
#define BUZZER_GND_PIN 7

String previousData = "";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRANSISTOR_BASE_PIN, OUTPUT);
  digitalWrite(TRANSISTOR_BASE_PIN, HIGH);
  pinMode(BUZZER_GND_PIN, OUTPUT);
  digitalWrite(BUZZER_GND_PIN, LOW);
  analogWrite(BUZZER_PIN, 0);
}

void loop() {
  String data = fetch("raw.githubusercontent.com", "/otcova/internet_remote_controler/main/state.txt");
  if (data != "" && previousData != data) {
    if (previousData != "") triggerSwitch();
    previousData = data;
  }
  sleep();
}

void triggerSwitch() {
  warning_beep();
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

void warning_beep() {
  for (int i = 0; i < 10; ++i) {
    analogWrite(BUZZER_PIN, 10);
    delay(100);
    analogWrite(BUZZER_PIN, 0);
    delay(30);
  }
}
