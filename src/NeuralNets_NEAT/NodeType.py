from enum import Enum, unique

@unique
class NodeType(Enum):
    INPUT = 1
    HIDDEN = 2
    OUTPUT = 3
