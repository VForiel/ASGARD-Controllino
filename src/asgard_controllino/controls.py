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

        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect((ip, port))
        self.client.settimeout(10)

    def clear_buffer(self):
        self.client.settimeout(1e-20)
        try:
            while True:
                data = self.client.recv(1024)
                if not data:
                    break
        except BlockingIOError:
            pass  # Rien à lire, c'est normal
        except TimeoutError:
            pass
        self.client.settimeout(10)

    def turn_on(self, key):
        self.clear_buffer()
        if key in CONNEXIONS:

            if key.startswith('8893-K-M '):
                if key.endswith('+') and self.get_status(k2 := key[:-1] + '-'):
                    raise IOError(f"Can't turn on '{key}' while '{k2}' is on")
                if key.endswith('-') and self.get_status(k2 := key[:-1] + '+'):
                    raise IOError(f"Can't turn on '{key}' while '{k2}' is on")

            self.client.sendall(bytes("o" + str(CONNEXIONS[key]) + "\n", "utf-8"))
            return self.client.recv(1024).decode().replace("\n", "").replace("\r", "")
        else:
            raise ValueError(f"Unkown device '{key}'")
    
    def turn_off(self, key):
        self.clear_buffer()
        if key in CONNEXIONS:
            self.client.sendall(bytes("c" + str(CONNEXIONS[key]) + "\n", "utf-8"))
            return self.client.recv(1024).decode().replace("\n", "").replace("\r", "")
        else:
            raise ValueError(f"Unknown device '{key}'")
        
    def get_status(self, key):
        self.clear_buffer()
        if key in CONNEXIONS:
            self.client.sendall(bytes("g" + str(CONNEXIONS[key]) + "\n", "utf-8"))
            r = self.client.recv(1024).decode().replace("\n", "").replace("\r", "")
            return True if r == "1" else False
        else:
            raise ValueError(f"Unknown device '{key}'")