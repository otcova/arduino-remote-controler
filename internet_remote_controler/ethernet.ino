#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 1, 1);
EthernetClient client;


struct Response {
  bool is_valid;
  bool switch_state;
  short sleep_for_minutes;
  static byte past_switch_state;
  Response(byte, byte);
  void use_response();
  void trigger_switch();
};

static byte Response::past_switch_state = 0;

Response::Response(byte switch_state_byte, byte sleep_time_byte) {
  is_valid = '0' <= switch_state && switch_state <= '9' && 
    '0' <= sleep_time_byte && sleep_time_byte <= '9';
    
  switch_state = switch_state_byte
  sleep_for_minutes = 1 << (sleep_time_byte - '0')
}

void Response::use_response() {
  past_switch_state = switch_state
}

bool Response::trigger_switch() {
  if (past_switch_state == 0) return false
  return past_switch_state == switch_state
}



void initializeEthernet() {
  if (Ethernet.begin(mac) == 0) {
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
      fatalError(F("Ethernet shield was not found"));
    if (Ethernet.linkStatus() == LinkOFF)
      fatalError(F("Ethernet cable is not connected."));
    Ethernet.begin(mac, ip, myDns);
  }
}

void sendHttpRequest(const char* host, const char* path) {
  // client.setTimeout(1000);
  client.connect(host, 80);
  client.print(F("GET "));
  
  client.print(path);
  client.println(F(" HTTP/1.1"));
  
  client.println(F("accept: accept: application/vnd.github.v3+json"));
  
  client.println(F("Clear-Site-Data: \"*\""));
  
  client.print(F("Host: "));
  client.println(host);
  
  client.println(F("Connection: close"));
  
  client.println();
}


Response reciveHttpResponse() {
  PRINTLN(F("> RECEIVE"));
  
  byte response_switch_state_byte = 0;
  byte response_sleep_time_byte = 0;
  
  while (client.connected()) {
    int len = client.available();
    if (len > 0) {
      byte buffer[128];
      if (len > 128) len = 128;
      client.read(buffer, len);
      if (len > 1) response_switch_state_byte = buffer[len-2];
      response_sleep_time_byte = buffer[len-1];
    }
  }
  client.stop();
  
  Response response = Response(response_switch_state_byte, response_sleep_time_byte);
  
  PRINTLN();
  PRINT(F("Data: "));
  PRINTLN(response.switchState);
  PRINTLN(response.sleepForMinutes);
  
  return response;
}

Response fetchGet(const char* host, const char* path) {
  initializeEthernet();
  PRINTLN(F("----- FETCH -----"));
  
  Response response = reciveHttpResponse();
  
  if (response.is_valid && response.trigger_switch()) {
    Response check_response = reciveHttpResponse();
    if (!check_response.is_valid || !response.trigger_switch())
      response.is_valid = false
  }
  
  return response;
}
