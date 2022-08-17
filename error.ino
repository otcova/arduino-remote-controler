#define SUCCESS true
#define ERROR false

void fatalError(const char* msg) {
  if (!Serial) Serial.begin(9600);
  while (!Serial);
  Serial.print("[FATAL-ERROR] ");
  Serial.println(msg);
  while (true);
}

void error(const char* msg) {
  if (!Serial) Serial.begin(9600);
  while (!Serial);
  Serial.print("[ERROR] ");
  Serial.println(msg);
}
