#include <SPI.h>
#include <Ethernet.h>
#include <EthernetServer.h>

byte mac[] = {0x50, 0xD7, 0x53, 0x00, 0xEB, 0xA7};    // MAC address (can be found on the Controllino)
IPAddress ip(192, 168, 1, 100);  // Adresse IP de l'Arduino
EthernetServer server(80);  // Serveur sur le port 5555

String inputString = ""; // Stocke la chaîne reçue
bool stringComplete = false; // Vérifie si l'entrée est complète

void setup() {
  // Initialisation de la connexion Ethernet
  Ethernet.begin(mac, ip);
  server.begin();
  
  // Initialisation des pins de l'Arduino
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); // Éteindre tous les pins au démarrage
  }
  
  Serial.begin(9600); // Pour le debug
  inputString.reserve(200); // Alloue de l'espace pour la chaîne
}

void loop() {
  // Vérifie si un client est connecté
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          stringComplete = true;
        } else {
          inputString += c;
        }

        // Lorsque la commande est complète, traite la chaîne
        if (stringComplete) {
          handleCommand(inputString, client);
          inputString = "";  // Réinitialise la chaîne
          stringComplete = false;
        }
      }
    }
    client.stop();  // Ferme la connexion
  }
}

void handleCommand(String command, EthernetClient client) {
  // Sépare la commande et le numéro de pin
  int spaceIndex = command.indexOf(' ');
  if (spaceIndex == -1) {
    client.println("Invalid command");
    return;
  }

  String action = command.substring(0, spaceIndex);
  int pin = command.substring(spaceIndex + 1).toInt();

  if (action == "ON") {
    digitalWrite(pin, HIGH);  // Allume le pin
    client.println("Pin " + String(pin) + " is ON");
  } else if (action == "OFF") {
    digitalWrite(pin, LOW);   // Éteint le pin
    client.println("Pin " + String(pin) + " is OFF");
  } else {
    client.println("Unknown command");
  }
}
