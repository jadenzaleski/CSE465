# Record Class
class Record:
    # Initialization for Record
    def __init__(self, parts):
        self.record_number = parts[0]
        self.zipcode = parts[1]
        self.city = parts[3]
        self.state = parts[4]
        self.lat = float(parts[6]) if parts[6] != '' else None
        self.lng = float(parts[7]) if parts[7] != '' else None

    # Override string method
    def __str__(self):
        return f'{self.record_number} {self.city} {self.state} {self.lat} {self.lng}'

    # Override logical ==
    def __eq__(self, other):
        if isinstance(other, Record):
            return (
                    self.record_number == other.record_number and
                    self.zipcode == other.zipcode and
                    self.city == other.city and
                    self.state == other.state and
                    self.lat == other.lat and
                    self.lng == other.lng
            )
        else:
            return False

    # See if two Records are equal in regard to zipcodes
    def same_zipcode(self, other):
        return self.zipcode == other.zipcode

    # Make the Record hashable
    def __hash__(self):
        return hash((self.record_number, self.zipcode, self.city, self.state, self.lat, self.lng))
