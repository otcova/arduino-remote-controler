#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 1, 1);
EthernetClient client;

void initializeEthernet() {
  if (Ethernet.begin(mac) == 0) {
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
      fatalError("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    if (Ethernet.linkStatus() == LinkOFF)
      fatalError("Ethernet cable is not connected.");
    Ethernet.begin(mac, ip, myDns);
  }
  // give the Ethernet shield time to initialize
  // TODO: try diferent values
  delay(500);
}

void sendHttpRequest(const char* host, const char* path) {
  if (client.connect(host, 80)) {
    client.print("GET ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("Connection: close");
    client.println();
  }
}

String reciveHttpResponce() {
  String response = "";
  
  while (client.connected()) {
    if (client.available() > 0)
      response += client.read();
  }
  
  client.stop();
  return response;
}

String fetch(const char* host, const char* path) {
  initializeEthernet();
  sendHttpRequest(host, path);
  return reciveHttpResponce();
}
