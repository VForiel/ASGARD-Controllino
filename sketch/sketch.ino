#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {0x50, 0xD7, 0x53, 0x00, 0xEB, 0xA7};    // MAC address (can be found on the Controllino)
IPAddress ip(172,16,8,200);                       // IP address (arbitrarily choosen)
EthernetServer server(23);                            // HTTP port

void setup() {

  for (int i=0; i<5; i++){
    digitalWrite(CONTROLLINO_D0, HIGH);
    digitalWrite(CONTROLLINO_R0, HIGH);
    delay(100);
    digitalWrite(CONTROLLINO_D0, LOW);
    digitalWrite(CONTROLLINO_R0, LOW);
    delay(100);
  }

  // Ethernet initialization
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);

  Serial.println("Server started. You can connect at:");
  Serial.println(Ethernet.localIP());
}

void loop() {

  // Listen resquests
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("Nouveau client connecté");

    // Wait for the request
    String request = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        // If the line is empty, the HTTP request is complete and can be treated
        if (c == '\n') {
          
          Serial.println("New Request:");
          Serial.print(request);
          
          if (request[0] == 'o') {
            int output = get_output(request);
            digitalWrite(output, HIGH);
            client.println("o");
            client.println("Ok");
          } 
          else if (request[0] == 'c') {
            int output = get_output(request);
            digitalWrite(output, LOW);
            client.println("c");
            client.println("Ok");
          }
          else if (request[0] == 'q'){
            client.stop();
            Serial.println("Client déconnecté");
          }
          else {
            client.println(request);
            client.println(request[0]);
            client.println("Err");
          }
          request = "";
        }
      }
    }
  }
}

int get_output(String request){
  int i = 0;
  String number;
  while (!isdigit(request[i]) && i < request.length()) {
      Serial.println("not digit");
      i++;
  }
  while (isdigit(request[i]) && i < request.length()) {
      Serial.println("digit");
      number += request[i];
      i++;
  }
  return number.toInt();
}