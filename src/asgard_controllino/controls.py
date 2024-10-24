import socket

# List of devices and the associated arduino pin
CONNEXIONS = {
    'MFF101/M 1-4' : 2,
    '8893-K-M 1+' : 3,
    '8893-K-M 1-' : 4,
    '8893-K-M 2+' : 5,
    '8893-K-M 2-' : 6,
    '8893-K-M 3+' : 7,
    '8893-K-M 3-' : 8,
    '8893-K-M 4+' : 9,
    '8893-K-M 4-' : 10,
    'LS16P' : 22,
    'DM1' : 42,
    'DM2' : 43,
    'DM3' : 44,
    'DM4' : 45,
    'XMC (BMX)' : 46,
    'XMC (BMY)' : 47,
    'XMC (BFO,SDL)' : 48,
    'USB hubs both' : 49,
    'MFF signal 1' : 77,
    'MFF signal 2' : 78,
    'MFF signal 3' : 79,
    'MFF signal 4' : 80,
}

# List of devices
def get_devices():
    return list(CONNEXIONS.keys())

class Controllino():
    def __init__(self, ip, port=23):
        self.ip = ip
        self.port = port

        self._maintain_connection = False
        self.client = None

    # Ensure the device is known
    def _ensure_device(self, key:str):
        if key not in CONNEXIONS:
            raise ValueError(f"Unkown device '{key}'")
    
    # Create a socket to communicate with the device
    def connect(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.settimeout(10)
        self.client.connect((self.ip, self.port))
        
    # Close the socket
    def disconnect(self):
        self.client.close()
        self.client = None

    # Maintain connexion if there is only one user
    @property
    def maintain_connection(self) -> bool:
        return self._maintain_connection
    @maintain_connection.setter
    def maintain_connection(self, value:bool):
        if value:
            self.connect()
        else:
            self.disconnect()
        self._maintain_connection = value

    # Clear the buffer before sending a command to avoid bug when reading the answer
    def _clear_buffer(self):
        # Set very very short timeout to avoid waiting for new data
        self.client.settimeout(1e-20)
        try:
            while True:
                data = self.client.recv(1024)
                if not data:
                    break
        except BlockingIOError:
            pass  # Nothing to read, normal
        except TimeoutError:
            pass # No answer, normal too

        # Reset timeout
        self.client.settimeout(10)

    # Send a command to the device
    def send_command(self, command:str) -> bool:
        # If the connection is not maintained, we need to connect before sending the command
        if self.client is None:
            self.connect()

        # Clear the buffer before sending the command
        self._clear_buffer()

        # Send the command
        self.client.sendall(bytes(command + "\n", "utf-8"))
        # Wait for the answer
        r = self.client.recv(1024).decode().replace("\n", "").replace("\r", "")

        # Disconnect to allow other users to send commands
        if not self.maintain_connection:
            self.disconnect()

        return bool(int(r)) # Convert the answer to a boolean

    # Command to turn on a device
    def turn_on(self, key:str) -> bool:
        self._ensure_device(key)

        # Manage linked devices to avoid conflicts
        if key.startswith('8893-K-M '):
            if key.endswith('+') and self.get_status(k2 := key[:-1] + '-'):
                raise IOError(f"Can't turn on '{key}' while '{k2}' is on")
            if key.endswith('-') and self.get_status(k2 := key[:-1] + '+'):
                raise IOError(f"Can't turn on '{key}' while '{k2}' is on")

        return self.send_command("o" + str(CONNEXIONS[key]))
    
    # Command to turn off a device
    def turn_off(self, key:str) -> bool:
        self._ensure_device(key)
        return self.send_command("c" + str(CONNEXIONS[key]))
        
    # Command to get the power status of a device
    def get_status(self, key:str) -> bool:
        self._ensure_device(key)
        return self.send_command("g" + str(CONNEXIONS[key]))
    
    # Command to set the piezo DAC value
    def set_piezo_dac(self, value:float) -> bool:
        if value < 0 or value > 1:
            raise ValueError("The value must be between 0 and 1")
        value = int(value*4095) # Convert the value to the 12 bits DAC range
        return self.send_command(f"a{value}")