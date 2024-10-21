import requests

class ASGARD_Controllino():
    def __init__(self, ip):
        self.ip = ip
        self.url = "http://" + self.ip + "/"

    def turn_on(self):
        return requests.get(self.url + "on")
    
    def turn_off(self):
        return requests.get(self.url + "off")