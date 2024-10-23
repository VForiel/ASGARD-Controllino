#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_MCP4728.h>

#define ANALOG_I2C_ADDR 40

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {0x50, 0xD7, 0x53, 0x00, 0xEB, 0xA7};    // MAC address (can be found on the Controllino)
IPAddress ip(172,16,8,200);                           // IP address (arbitrarily choosen)
EthernetServer server(23);                            // HTTP port
Adafruit_MCP4728 mcp;

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
  Serial.begin(9600);

  // join I2C bus (address optional for master)
  // !!! Where do we set the speed??
  // Wire.begin(); 

  // Try to initialize! 
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip"); 
    while (1) {
      delay(10); 
    }
  }
  mcp.setChannelValue(MCP4728_CHANNEL_B, 0);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // Start listening for clients
  server.begin();

  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {

  // Listen resquests
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("Client connected");

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

          // Command is open|close|get|analog
          if (c == 'o' || c == 'c' || c == 'g' || c == 'a') {
            int value = get_value(request);

            if (value != -1) {
              if (c == 'o') {
                if (is_pwm(value)){
                  analogWrite(value, int(255*9/24));
                } else {
                  digitalWrite(value, HIGH);
                }
                client.println("1");
              } else if (c == 'c') {
                digitalWrite(value, LOW);
                client.println("1");
              } else if (c == 'g') {
                if (is_pwm(value)){
                  client.println(min(1,pulseIn(value, HIGH)));
                } else {
                  client.println(String(digitalRead(value)));
                }
              } else if (c == 'a') {
                mcp.setChannelValue(MCP4728_CHANNEL_B, value);
                client.println("1");
              }
            } else {
              client.println("0");
            }
          } else if (c == 'q'){
            client.stop();
          } else {
            client.println("0");
          }
          request = "";
        }
      }
    }
    Serial.println("Client diconnected");
  }
}

bool is_pwm(int pin){
  if (pin >= 2 && pin <= 10){
    return true;
  } else {
    return false;
  }
}

int get_value(String request){
  int i = 1;
  String svalue;
  int value;
  char command = request[0];

  if (sizeof(request) > 1 && isdigit(request[i])) {
    // Read the value at the end of the command
    while (isdigit(request[i]) && i < request.length()) {
        svalue += request[i];
        i++;
    }
    value = svalue.toInt();

    // open | close | get commands -> get pin number
    if (command == 'o' || command == 'c' || command == 'g'){
      // Check that the pin number is correct
      int arr[] = {2,3,4,5,6,7,8,9,10,22,42,43,44,45,46,47,48,49,77,78,79,80}; // Used pins
      for(int i = 0; i < sizeof(arr); ++i) {
        if(arr[i] == value) {
            return value;
        }
      }
    }
    
    // analog command -> get value
    if (command == 'a'){
      // Check that the value is between 0 and 4096
      if (value >= 0 && value < 4096){
        return value;
      }
    }

    return -1; // Invalid value
  } else {
    return -1; // No value
  }
}