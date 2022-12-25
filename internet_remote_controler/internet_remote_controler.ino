#include "LowPower.h"

//#define LOG

#define SWITCH_PIN 5
#define ETHERNET_SWITCH 2
#define BUZZER_PIN 9
#define BUZZER_GND_PIN 7

#ifdef LOG
#define SETUP_PRINT(x) Serial.begin(9600); while (!Serial)
#define PRINTLN(x) Serial.println(x)
#define PRINT(x) Serial.print(x)
#define FATAL_ERROR(x) {PRINT(F("[FATAL-ERROR] ")); PRINTLN(x); powerOff();}
#define PRINT_ERROR(x) {PRINT(F("[ERROR] ")); PRINTLN(x);}
#else
#define SETUP_PRINT(x)
#define PRINTLN(x)
#define PRINT(x)
#define FATAL_ERROR(x) {powerOff();}
#define PRINT_ERROR(x)
#endif

enum Response {
    Invalid, // Fetch resulted in error
    Change, // The state.txt have chaned => open pc
    Continue, // The state.txt haven't changed => do not open pc
};

enum Result {
    Ok,
    Error,
};

int error_count = 0;

void(* resetArduino) (void) = 0;

void setup() {
    pinMode(SWITCH_PIN, OUTPUT);
    digitalWrite(SWITCH_PIN, LOW);
    
    pinMode(ETHERNET_SWITCH, OUTPUT);
    digitalWrite(ETHERNET_SWITCH, LOW);
    
    pinMode(BUZZER_GND_PIN, OUTPUT);
    digitalWrite(BUZZER_GND_PIN, LOW);
    
    analogWrite(BUZZER_PIN, 0);
    
    SETUP_PRINT();
    PRINTLN(":)");
}

void loop() {
    power_on_ethernet();
    Response response = fetchGet();
    power_off_ethernet();
    
    if (response == Response::Invalid) {
        if (++error_count > 2) resetArduino();
        PRINT_ERROR(F("Invalid fetch response"));
        sleep(1);
    } else {
        error_count = 0;
        if (response == Response::Change) triggerSwitch();
    }
    
    sleep(2);
}

void triggerSwitch() {
    warning_beep();
    digitalWrite(SWITCH_PIN, HIGH);
    delay(800);
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

void power_on_ethernet() {
    digitalWrite(ETHERNET_SWITCH, HIGH);
    delay(1000);
}

void power_off_ethernet() {
    digitalWrite(ETHERNET_SWITCH, LOW);
    delay(500);
}
