#include "LowPower.h"

void sleep() {
  digitalWrite(TRANSISTOR_BASE_PIN, LOW);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  digitalWrite(TRANSISTOR_BASE_PIN, HIGH);
}
