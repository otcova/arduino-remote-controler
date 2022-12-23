#include "LowPower.h"

#define LOG

#define SWITCH_PIN 5
#define TRANSISTOR_BASE_PIN 2
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
#define FATAL_ERROR(x) powerOff()
#define PRINT_ERROR(x)
#endif

enum Response {
    Invalid, // Fetch resulted in error
    Change, // The state.txt have chaned => open pc
    Continue, // The state.txt haven't changed => do not open pc
};

void setup() {
  pinMode(SWITCH_PIN, OUTPUT);
  
  pinMode(TRANSISTOR_BASE_PIN, OUTPUT);
  digitalWrite(TRANSISTOR_BASE_PIN, HIGH);
  
  pinMode(BUZZER_GND_PIN, OUTPUT);
  digitalWrite(BUZZER_GND_PIN, LOW);
  
  analogWrite(BUZZER_PIN, 0);

  SETUP_PRINT();
  PRINTLN(":)");
}

void loop() {
  Response response = fetchGet("internet-remote.ddns.net", "/state.txt");
  if (response == Response::Change) triggerSwitch();
  else if (response == Response::Invalid) {
    PRINT_ERROR(F("Invalid fetch response"));
    sleep(9);
  } else sleep(3);
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
