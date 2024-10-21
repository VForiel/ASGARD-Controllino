#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {0x50, 0xD7, 0x53, 0x00, 0xEB, 0xA7};    // MAC address (can be found on the Controllino)
IPAddress ip(192, 168, 1, 177);                       // IP address (arbitrarily choosen)
EthernetServer server(80);                            // HTTP port

void setup() {
  // put your setup code here, to run once:

  pinMode(CONTROLLINO_D0, OUTPUT);
  pinMode(CONTROLLINO_D1, OUTPUT);
  pinMode(CONTROLLINO_D2, OUTPUT);
  pinMode(CONTROLLINO_D3, OUTPUT);

  // Ethernet initialization
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);

  // Ethernet will control the pin 5
  pinMode(CONTROLLINO_D5, OUTPUT);

  Serial.println("Server started. You can connect at:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(CONTROLLINO_D0, HIGH);
  delay(100);
  digitalWrite(CONTROLLINO_D0, LOW);
  delay(100);
  digitalWrite(CONTROLLINO_D1, HIGH);
  delay(100);
  digitalWrite(CONTROLLINO_D1, LOW);
  delay(100);
  digitalWrite(CONTROLLINO_D2, HIGH);
  delay(100);
  digitalWrite(CONTROLLINO_D2, LOW);
  delay(100);
  digitalWrite(CONTROLLINO_D3, HIGH);
  delay(100);
  digitalWrite(CONTROLLINO_D3, LOW);
  delay(100);

  // Listen resquests
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("Nouveau client connecté");

    // Wait for the request
    boolean currentLineIsBlank = true;
    String httpRequest = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        httpRequest += c;

        // If the line is empty, the HTTP request is complete and can be treated
        if (c == '\n' && currentLineIsBlank) {
          
          if (httpRequest.indexOf("GET /on") != -1) {
            digitalWrite(CONTROLLINO_D5, HIGH);
          } 
          else if (httpRequest.indexOf("GET /off") != -1) {
            digitalWrite(CONTROLLINO_D5, LOW);
          }

          // Answer the client
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<h1>Arduino Ethernet Control</h1>");
          client.println("<p>Envoyer GET /on pour allumer ou GET /off pour éteindre.</p>");
          client.println("</html>");
          
          break;
        }

        // Gérer les retours chariot
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    // Fermer la connexion
    delay(1);
    client.stop();
    Serial.println("Client déconnecté");
  }
}