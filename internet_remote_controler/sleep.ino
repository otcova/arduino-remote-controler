#include "LowPower.h"

void sleep(short minutes) {
    PRINTLN("sleep started");
    power_off_ethernet();
    for (int n = 0; n < minutes; ++n) {
        for (byte i = 0; i < 7; ++i) {
            LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
        }
        LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    }
    delay(1000);
    PRINTLN("sleep ended");
}

void powerOff() {
    power_off_ethernet();
    delay(1000);
    while (true) {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
}
