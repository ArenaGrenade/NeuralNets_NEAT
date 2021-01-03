from Evaluator import Evaluator
from AbstractNodeGene import AbstractNodeGene
from NodeType import NodeType
from ConnectionGene import ConnectionGene
from Genome import Genome
from random import Random
from Innovator import Innovator
import matplotlib.pyplot as plt
from copy import deepcopy
from dotted_dict import DottedDict
import progressbar

class TestNode(AbstractNodeGene):
    def __init__(self, id, type):
        super(TestNode, self).__init__(id, type)

    def forward(self):
        pass

def generate_complete_genome(id, n, r, ir):
    g = Genome(id)

    for i in range(n):
        g.add_node_gene(TestNode(i, r.choice(list(NodeType))))
    
    for i in range(n):
        for j in range(i, n):
            w = r.random()
            a = r.random() < 0.7
            inr = ir.next_innovation_number((i, j))
            g.add_connection_gene(ConnectionGene(i, j, w, a, inr))
    
    return g

def generate_genome(id, n, r, ir):
    g = Genome(id)

    max_c = int(n * (n - 1) / 2)
    c = r.randint(max_c - 1, max_c)

    for i in range(n):
        g.add_node_gene(TestNode(i, r.choice(list(NodeType))))

    for _ in range(c):
        i = r.randint(0, n - 1)
        o = r.randint(0, n - 1)
        w = r.random()
        a = r.random() < 0.7
        inr = ir.next_innovation_number((i, o))
        g.add_connection_gene(ConnectionGene(i, o, w, a, inr))
    
    return g

def simple_trial():
    i = Innovator()
    r = Random()
    config = {
        "MUTATION_RATE": 0.0,
        "CONNECTION_MUTATION_RATE": 0.0,
        "NODE_MUTATION_RATE": 0.0,
        "DISABLED_GENE_INHERITING_CHANCE": 1.0,
    }

    config = DottedDict(config)

    nodes = 4
    to_remove = 2

    g1 = generate_complete_genome(1, nodes, r, i)
    g2 = generate_complete_genome(2, nodes, r, i)

    g1.fitness = 10.0
    g2.fitness = 0.0

    for key in r.sample(g1.connection_genes.keys(), to_remove):
        del g1.connection_genes[key]

    for key in r.sample(g2.connection_genes.keys(), to_remove):
        del g2.connection_genes[key]
    
    gc = Genome.generate_offspring(g1, 3, r, [TestNode], i, config, genomeB=g2)

    g1.vizualize_genome(1, "g1")
    g2.vizualize_genome(2, "g2")
    gc.vizualize_genome(3, "gc")

    plt.show()

def sample_fitness(genome):
    sum_weight = sum([conn.weight for conn in genome.connection_genes.values()])
    if sum_weight > 50:
        return 1.0
    return 0.0

if __name__ == "__main__":
    i = Innovator()
    r = Random()
    e = Evaluator(sample_fitness, i, [TestNode])

    iters = 100

    e.initialize_population(size=(1, 2))
    for _ in progressbar.progressbar(range(iters)):
        e.evolve_generation()

    pop = list(e.previous_generation.items())
    pop.sort(key=lambda x: x[1].fitness, reverse=True)
    for (genome_id, genome) in pop[:1]:
        genome.vizualize_genome(genome_id, "genome {} -- {}".format(genome_id, 1 / genome.fitness if genome.fitness > 0 else 0))
    
    plt.show()
