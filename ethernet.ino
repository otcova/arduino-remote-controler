#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 1, 1);
EthernetClient client;

void initializeEthernet() {
  if (Ethernet.begin(mac) == 0) {
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
      fatalError("Ethernet shield was not found");
    if (Ethernet.linkStatus() == LinkOFF)
      fatalError("Ethernet cable is not connected.");
    Ethernet.begin(mac, ip, myDns);
  }
}

void sendHttpRequest(const char* host, const char* path) {
  client.connect(host, 80);
  client.print("GET ");
  client.print(path);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();
}

char reciveHttpResponce() {
  while (client.connected()) {
    int len = client.available();
    if (len > 0) {
      char data = client.read();
      PRINTLN("Data: ");
      PRINTLN(data);
      client.stop();
      return data;
    }
  }
  
  client.stop();
  return 0;
}

char fetchGet(const char* host, const char* path) {
  initializeEthernet();
  sendHttpRequest(host, path);
  return reciveHttpResponce();
}
