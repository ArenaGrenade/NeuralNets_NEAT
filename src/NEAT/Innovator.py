class Innovator:
    def __init__(self):
        self.present_count = 1
        self.innovations = {}

    def next_innovation_number(self, connection):
        if connection not in self.innovations:
            self.innovations[connection] = self.present_count
            self.present_count += 1
        return self.innovations[connection]
