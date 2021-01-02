class ConnectionGene:
    def __init__(self, in_node: int, out_node: int, weight: float, is_active: bool, innovation_number: int):
        # Error Handling
        if in_node < 0:
            raise ValueError("in_node cannot be negative")
        if out_node < 0:
            raise ValueError("out_node cannot be negative")
        if innovation_number <= 0:
            raise ValueError("innovation_number needs to be positive")

        self.in_node = in_node
        self.out_node = out_node

        self.weight = weight
        self.is_active = is_active

        self.innovation_number = innovation_number

    def set_weight(self, weight):
        self.weight = weight

    def disable(self):
        self.is_active = False
