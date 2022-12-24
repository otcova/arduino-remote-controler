#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 1, 1);
EthernetClient client;

byte past_state = 0;


void initializeEthernet()
{
    if (Ethernet.begin(mac) == 0) {
        if (Ethernet.hardwareStatus() == EthernetNoHardware)
            FATAL_ERROR(F("Ethernet shield was not found"));
        if (Ethernet.linkStatus() == LinkOFF)
            FATAL_ERROR(F("Ethernet cable is not connected."));
        PRINTLN("begin");
        Ethernet.begin(mac, ip, myDns);
    }
    PRINTLN(F("ETHERNET INIT DONE"));
}

Result sendHttpRequest(const char* host, const char* path)
{
    if (!client.connect(host, 80)) {
        PRINT_ERROR(F("Conection failed"));
        return Result::Error;
    }

    client.print(F("GET "));
    client.print(path);
    client.println(F(" HTTP/1.1"));

    // client.println(F("accept: accept: application/vnd.github.v3+json"));
    // client.println(F("Clear-Site-Data: \"*\""));

    client.print(F("Host: "));
    client.println(host);

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

Response fetchGet(const char* host, const char* path)
{
    PRINTLN(F("----- FETCH -----"));
    initializeEthernet();
    if (sendHttpRequest(host, path) == Result::Error) return Response::Invalid;
    return reciveHttpResponse();
}
