#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 1, 1);
EthernetClient client;

byte past_state = 0;


Result initializeEthernet()
{   
    if (Ethernet.begin(mac, 5000, 1000) == 0) {
        PRINT_ERROR(F("Failed to configure Ethernet using DHCP"));
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            FATAL_ERROR(F("Ethernet shield was not found"));
        }
        if (Ethernet.linkStatus() == LinkOFF) {
            FATAL_ERROR(F("Ethernet cable is not connected."));
        }
        return Result::Error;
    }
    PRINTLN(F("ETHERNET INIT DONE"));
    return Result::Ok;
}

Result sendHttpRequest()
{
    if (!client.connect("internet-remote.ddns.net", 80)) {
        PRINT_ERROR(F("Conection failed"));
        return Result::Error;
    }

    client.println(F("GET /state.txt HTTP/1.1"));
    client.println(F("Host: internet-remote.ddns.net"));
    client.println(F("Connection: close"));
    client.println();
    return Result::Ok;
}

Response reciveHttpResponse()
{
    PRINTLN(F("> RECEIVE"));

    const int buffer_len = 7;
    byte buffer[buffer_len];
    
    while (client.connected()) {
        int len = client.available();
        if (len <= 0) continue;
        
        PRINT("INPUT BYTES: ");
        PRINTLN(len);

        // Skip headers
        byte a = 0, b = 0, c = 0;
        while (a != '\n' || b != '\r' || c != '\n') {
            c = b;
            b = a;
            client.read(&a, 1);
            PRINT(char(a));
            if (--len < buffer_len) {
                client.stop();
                return Response::Invalid;
            }
        }

        // Read response body
        client.read(buffer, buffer_len);
        break;
    }

    client.stop();

    PRINTLN();
    PRINT(F("Data: "));
    for (int i = 0; i < buffer_len; ++i) PRINT(char(buffer[i]));
    PRINTLN(';');
    
    if (buffer[0] == 's'
        && buffer[1] == 't'
        && buffer[2] == 'a'
        && buffer[3] == 't'
        && buffer[4] == 'e'
        && buffer[5] == '=') {

        byte state = buffer[6];

        if (past_state == 0 || past_state == state) {
            past_state = state;
            return Response::Continue;
        }

        past_state = state;
        return Response::Change;
    }
    
    return Response::Invalid;
}

Response fetchGet()
{
    PRINTLN(F("----- FETCH -----"));
    if (initializeEthernet() == Result::Error) return Response::Invalid;
    if (sendHttpRequest() == Result::Error) return Response::Invalid;
    return reciveHttpResponse();
}
