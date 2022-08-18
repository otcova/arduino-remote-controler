#include "LowPower.h"

void sleep() {
  digitalWrite(TRANSISTOR_BASE_PIN, LOW);
  PRINTLN("sleep");
  delay(1000);
  for (int i = 0; i < 10; ++i)
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_ON);
  delay(1000);
  digitalWrite(TRANSISTOR_BASE_PIN, HIGH);
}

void powerOff() {
  digitalWrite(TRANSISTOR_BASE_PIN, LOW);
  while (true) {
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_ON);
  }
}
