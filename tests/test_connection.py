import pytest
from src.NEAT.ConnectionGene import ConnectionGene

@pytest.mark.parametrize(
    "i, o, w, a, n",
    [
        (-1, 0, 0.0, False, 1),
        (0, -1, 0.0, False, 1),
        (0, 0, 0.0, False, -1),
        (0, 0, 0.0, False, 0),
    ],
)
def test_invalid_creation(i: int, o: int, w: float, a: bool, n: int) -> None:
    with pytest.raises(ValueError):
        ConnectionGene(i, o, w, a, n)

@pytest.mark.parametrize("w", [0.0, -100000000.0, 100000000.0,])
def test_set_weight(w) -> None:
    c = ConnectionGene(4, 3, 2.0, True, 1)
    c.set_weight(w)
    assert c.weight == w

def test_disable_connection() -> None:
    c = ConnectionGene(4, 3, 2.0, True, 1)
    c.disable()
    assert c.is_active == False
