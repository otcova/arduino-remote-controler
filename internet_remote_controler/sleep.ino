#include "LowPower.h"

void sleep(short minutes) {
  PRINTLN("sleep started");
  digitalWrite(TRANSISTOR_BASE_PIN, LOW);
  delay(1000);
  for (int n = 0; n < minutes; ++n) {
    for (byte i = 0; i < 7; ++i) {
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
  
  delay(1000);
  PRINTLN("sleep ended");
  digitalWrite(TRANSISTOR_BASE_PIN, HIGH);
  delay(1000);
}

void powerOff() {
  digitalWrite(TRANSISTOR_BASE_PIN, LOW);
  while (true) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
