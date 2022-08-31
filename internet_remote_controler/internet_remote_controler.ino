#include "LowPower.h"

#define SWITCH_PIN 5
#define TRANSISTOR_BASE_PIN 2
#define BUZZER_PIN 9
#define BUZZER_GND_PIN 7

#define SETUP_PRINT(x) Serial.begin(9600); while (!Serial)
#define PRINTLN(x) Serial.println(x)
#define PRINT(x) Serial.print(x)
// #define SETUP_PRINT(x)
//#define PRINTLN(x)
//#define PRINT(x)


struct Response {
  bool is_valid;
  bool switch_state;
  short sleep_for_minutes;
  static byte past_switch_state;
  Response(byte, byte);
  void use_response();
  void trigger_switch();
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
  Response response = fetchGet("api.github.com", "/repos/otcova/internet-remote-controler/contents/arduino-data.txt");
  if (response.is_valid && response.trigger_switch()) {
    triggerSwitch();
  }
  sleep(response.sleep_for_minutes);
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
