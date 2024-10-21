import zmq

class ASGARD_Controllino():
    def __init__(self, ip, port=80):
        self.ip = ip
        self.port = port

        # Configuration de la socket ZMQ
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)  # REQ : socket de requête
        self.socket.connect(f"tcp://{ip}:{port}")  # L'adresse IP de votre Arduin

    # Fonction pour allumer un pin spécifique
    def turn_on(self, pin):
        message = f"ON {pin}"
        self.socket.send_string(message)  # Envoie de la commande à l'Arduino
        response = self.socket.recv_string()  # Réception de la réponse de l'Arduino
        print(f"Arduino Response: {response}")

    # Fonction pour éteindre un pin spécifique
    def turn_off(self, pin):
        message = f"OFF {pin}"
        self.socket.send_string(message)  # Envoie de la commande à l'Arduino
        response = self.socket.recv_string()  # Réception de la réponse de l'Arduino
        print(f"Arduino Response: {response}")
