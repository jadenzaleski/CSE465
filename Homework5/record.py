# Record Class
class Record:
    def __init__(self, parts):
        self.record_number = parts[0]
        self.zipcode = parts[1]
        self.city = parts[3]
        self.state = parts[4]
        self.lat = float(parts[6]) if parts[6] != '' else None
        self.lng = float(parts[7]) if parts[7] != '' else None

    def __str__(self):
        return f'{self.record_number} {self.city} {self.state} {self.lat} {self.lng}'
