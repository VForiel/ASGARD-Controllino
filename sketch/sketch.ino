#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {0x50, 0xD7, 0x53, 0x00, 0xEB, 0xA7};    // MAC address (can be found on the Controllino)
IPAddress ip(172,16,8,200);                           // IP address (arbitrarily choosen)
EthernetServer server(23);                            // HTTP port

void setup() {

  for (int i=0; i<3; i++){
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
          
          // Parse the request ------------------------------------------------

          char c = request[0];

          // Command is open|close|get
          if (c == 'o' || c == 'c' || c == 'g') {
            int pin = get_pin(request);

            if (pin != 0) {
              if (c == 'o') {
                if (is_pwm(pin)){
                  analogWrite(pin, int(255*9/12));
                } else {
                  digitalWrite(pin, HIGH);
                }
                client.println("Ok");
              } else if (c == 'c') {
                digitalWrite(pin, LOW);
                client.println("Ok");
              } else if (c == 'g') {
                if (is_pwm(pin)){
                  client.println(min(1,pulseIn(pin, HIGH)));
                } else {
                  client.println(String(digitalRead(pin)));
                }
              }
            } else {
              client.println("Err");
            }

          } else if (c == 'q'){
            client.stop();
            Serial.println("Client déconnecté");

          } else {
            client.println("Err");
          }
          request = "";
        }
      }
    }
  }
}

int get_pin(String request){
  int i = 0;
  String pin;
  while (!isdigit(request[i]) && i < request.length()) {
      i++;
  }
  if (isdigit(request[i])) {
    while (isdigit(request[i]) && i < request.length()) {
        pin += request[i];
        i++;
    }
    return pin.toInt();
  } else {
    return 0;
  }
}

bool is_pwm(int pin){
  if (pin >= 2 && pin <= 10){
    return true;
  } else {
    return false;
  }
}