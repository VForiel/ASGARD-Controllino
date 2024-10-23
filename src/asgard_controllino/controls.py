import socket

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

def get_devices():
    return list(CONNEXIONS.keys())

class Controllino():
    def __init__(self, ip, port=23):
        self.ip = ip
        self.port = port

        self._maintain_connection = False
        self.client = None

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

    def _ensure_device(self, key):
        if key not in CONNEXIONS:
            raise ValueError(f"Unkown device '{key}'")
        
    def connect(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.settimeout(10)
        self.client.connect((self.ip, self.port))
        
    def disconnect(self):
        self.client.close()
        self.client = None

    @property
    def maintain_connection(self):
        return self._maintain_connection

    @maintain_connection.setter
    def maintain_connection(self, value):
        if value:
            self.connect()
        else:
            self.disconnect()
        self._maintain_connection = value

    def turn_on(self, key):

        if self.client is None:
            self.connect()

        self._clear_buffer()
        self._ensure_device(key)

        # Manage linked devices
        if key.startswith('8893-K-M '):
            if key.endswith('+') and self.get_status(k2 := key[:-1] + '-'):
                raise IOError(f"Can't turn on '{key}' while '{k2}' is on")
            if key.endswith('-') and self.get_status(k2 := key[:-1] + '+'):
                raise IOError(f"Can't turn on '{key}' while '{k2}' is on")
            
        if self.client is None:
            self.connect()

        # Send turn on command
        self.client.sendall(bytes("o" + str(CONNEXIONS[key]) + "\n", "utf-8"))
        r = self.client.recv(1024).decode().replace("\n", "").replace("\r", "")

        if not self.maintain_connection:
            self.disconnect()

        return r
    
    def turn_off(self, key):

        if self.client is None:
            self.connect()

        self._clear_buffer()
        self._ensure_device(key)
        
        self.client.sendall(bytes("c" + str(CONNEXIONS[key]) + "\n", "utf-8"))
        r = self.client.recv(1024).decode().replace("\n", "").replace("\r", "")

        if not self.maintain_connection:
            self.disconnect()
        
        return r
        
    def get_status(self, key):
        
        if self.client is None:
            self.connect()
            
        self._clear_buffer()
        self._ensure_device(key)

        self.client.sendall(bytes("g" + str(CONNEXIONS[key]) + "\n", "utf-8"))
        r = self.client.recv(1024).decode().replace("\n", "").replace("\r", "")
    
        if not self.maintain_connection:
            self.disconnect()
        
        return bool(int(r))