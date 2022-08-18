#define SUCCESS true
#define ERROR false

void fatalError(const char* msg) {
  PRINT("[FATAL-ERROR] ");
  PRINTLN(msg);
  powerOff();
}

void error(const char* msg) {
  PRINT("[ERROR] ");
  PRINTLN(msg);
}
