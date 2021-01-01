from Genome import Genome
from AbstractNodeGene import AbstractNodeGene
from ConnectionGene import ConnectionGene
from NodeType import NodeType
import random
import matplotlib.pyplot as plt
from Innovator import Innovator
from Evaluator import Evaluator
from copy import deepcopy

class Node(AbstractNodeGene):
    def __init__(self, id, node_type):
        super().__init__(id, node_type)
    
    def forward(self):
        pass

def generate_genome(nodes, innov):
    g = Genome()

    conns = random.randint(int(nodes / 2), int((nodes * (nodes - 1)) / 2))

    for i in range(nodes):
        g.add_node_gene(Node(i, random.choice(list(NodeType))))

    for c in range(conns):
        connection = (random.randint(0, nodes - 1), random.randint(0, nodes - 1))
        g.add_connection_gene(ConnectionGene(*connection, random.random(), True, innov.next_innovation_number(connection)))
    
    return g

def simple_fitness(genome):
    diff = 100 - len(genome.connection_genes)
    if diff > 0:
        return 1000 * (1 / diff)
    else:
        return -1000

def print_sizes(evaluator):
    print("###############################################################################")
    for index, genome in enumerate(evaluator.previous_generation):
        print("conns ", index, ": ", len(genome.genome.connection_genes), " with fitness ", evaluator.fitness_function(genome.genome))
    print("###############################################################################")

if __name__ == '__main__':
    """
    i = Innovator()
    r = random.Random()
    g1 = generate_genome(4, i)
    g1.vizualize_genome(1, "G1")
    g1.add_connection_mutation(r, i)
    g1.add_connection_mutation(r, i)
    g1.add_connection_mutation(r, i)
    g1.add_connection_mutation(r, i)
    g1.vizualize_genome(2, "G2")

    plt.show()
    """
    i = Innovator()
    e = Evaluator(simple_fitness, i)

    genomes = [generate_genome(10, i) for _ in range(10)]

    e.initialize_population(genomes)

    for _ in range(100):
        e.evaluate_generation()
        print("Generation ", _, ": with best indiv ", len(e.best_individual.genome.connection_genes), " and best fitness ", e.best_individual.fitness)
        # print_sizes(e)
