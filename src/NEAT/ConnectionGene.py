class ConnectionGene:
    def __init__(self, in_node, out_node, weight, is_active, innovation_number):
        self.in_node = in_node
        self.out_node = out_node

        self.weight = weight
        self.is_active = is_active

        self.innovation_number = innovation_number

    def set_weight(self, weight):
        self.weight = weight

    def disable(self):
        self.is_active = False
