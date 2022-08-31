#include "LowPower.h"

void sleep(short minutes) {
  digitalWrite(TRANSISTOR_BASE_PIN, LOW);
  PRINTLN("sleep started");
  delay(500);
  for (short n = 0; n < minutes; ++n) {
    for (short i = 0; i < 7; ++i) {
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
  delay(500);
  PRINTLN("sleep ended");
  digitalWrite(TRANSISTOR_BASE_PIN, HIGH);
}

void powerOff() {
  digitalWrite(TRANSISTOR_BASE_PIN, LOW);
  while (true) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
