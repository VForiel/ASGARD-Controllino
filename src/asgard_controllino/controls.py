import requests

CONNEXIONS = {
    "D0": 2,
    "D1": 3,
    "D2": 4,
    "D3": 5,
    "D4": 6,
    "D5": 7,
    "D6": 8,
    "D7": 9,
    "D8": 10,
    "D9": 11,
    "D10": 12,
    "D11": 13,
    "D12": 42,
    "D13": 43,
    "D14": 44,
    "D15": 45,
    "D16": 46,
    "D17": 47,
    "D18": 48,
    "D19": 49,
    "D20": 77,
    "D21": 78,
    "D22": 79,
    "D23": 80,
    "R0": 22,
}

def get_devices():
    return list(CONNEXIONS.keys())

class Controllino():
    def __init__(self, ip, port=23):
        self.ip = ip
        self.port = port

    def turn_on(self, key):
        if key in CONNEXIONS:
            return requests.get("o" + str(CONNEXIONS[key]))
        else:
            raise ValueError(f"Unkown device '{key}'")
    
    def turn_off(self, key):
        if key in CONNEXIONS:
            return requests.get("c" + str(CONNEXIONS[key]))
        else:
            raise ValueError(f"Unknown device '{key}'")
        
    def get_status(self, key):
        if key in CONNEXIONS:
            return requests.get("g" + str(CONNEXIONS[key]))
        else:
            raise ValueError(f"Unknown device '{key}'")