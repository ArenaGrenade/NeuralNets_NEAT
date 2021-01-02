from abc import ABC, abstractmethod

class AbstractNodeGene(ABC):
    @abstractmethod
    def __init__(self, id, node_type):
        self._id = id
        self._type = node_type

    @abstractmethod
    def forward(self):
        raise NotImplementedError("This needs to be implemented")
