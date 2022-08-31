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
  client.println("Clear-Site-Data: \"*\"");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();
}

char reciveHttpResponce() {
  PRINTLN("> RECEIVE");
  char data = 0;
  while (client.connected()) {
    int len = client.available();
    if (len > 0) {
      byte buffer[150];
      if (len > 150) len = 150;
      client.read(buffer, len);
      data = buffer[len-1];
      Serial.write(buffer, len);
    }
  }
  client.stop();

  PRINTLN();
  PRINT("Data: ");
  PRINTLN(data);
  
  return data;
}

char fetchGet(const char* host, const char* path) {
  initializeEthernet();
  PRINTLN("----- FETCH -----");
  char data = 0;
  bool receivedDataIsStable= true;
  
  for (int n = 0; n < 10; ++n) {
    receivedDataIsStable = true;
    
    sendHttpRequest(host, path);
    data = reciveHttpResponce();
    
    if (data != '0' && data != '1') {
      receivedDataIsStable = false;
      data = 0;
    } else {
      for (int i = 0; i < 4; ++i) {
        sendHttpRequest(host, path);
        if (data != reciveHttpResponce())
          receivedDataIsStable = false;
      }
    }
    
    if (receivedDataIsStable)
      break;
    delay(1000);
  }
  
  return data;
}
