from ConnectionGene import ConnectionGene
from AbstractNodeGene import AbstractNodeGene
from NodeType import NodeType
from utils.graphing import viz_graph
import networkx as nx
import matplotlib.pyplot as plt
from itertools import product
from copy import deepcopy
from math import fabs

class Genome:
    def __init__(self, id):
        self.node_genes = {}
        self.connection_genes = {}
        self.config = {}
        self.fitness = 0.0

        self.id = id

    def add_node_gene(self, gene):
        if issubclass(type(gene), AbstractNodeGene):
            self.node_genes[gene._id] = gene
        else:
            raise TypeError("Expected gene to be an AbstractNodeGene")

    def add_connection_gene(self, gene):
        if isinstance(gene, ConnectionGene):
            self.connection_genes[gene.innovation_number] = gene
        else:
            raise TypeError("The gene object provided must be a ConnectionGene")
    
    def add_weight_mutation(self, random_gen):
        for id, connection in self.connection_genes.items():
            if random_gen.random() < self.config["PROBABILITY_PERTURBING"]:
                connection.weight *= random_gen.normalvariate(0.0, 1.0)
            else:
                connection.weight = random_gen.uniform(self.config["PROB_MIN"], self.config["PROB_MAX"])
    
    def add_connection_mutation(self, random_gen, innovator):
        #! Useless config variables
        PROB_MIN = -1.0
        PROB_MAX = 1.0
        #!
        in_nodes = self.node_genes.keys()
        out_nodes = [node for node in in_nodes if self.node_genes[node]._type != NodeType.INPUT]

        selected_connection = None
        present_connections = [(node[1].in_node, node[1].out_node) for node in self.connection_genes.items()]
        random_gen.shuffle(present_connections)
        for connection in product(in_nodes, out_nodes):
            if connection[0] == connection[1]:
                continue
            if connection not in present_connections:
                selected_connection = connection
                break
        
        if not selected_connection:
            return
        weight = random_gen.uniform(PROB_MIN, PROB_MAX)
        innovation_number = innovator.next_innovation_number(selected_connection)
        connection = ConnectionGene(*selected_connection, weight, True, innovation_number)
        self.connection_genes[innovation_number] = connection

    def add_node_mutation(self, random_gen, node_classe, innovator):
        if not node_classes:
            return

        active_connections = filter(lambda gene: gene.is_active, self.connectionn_genes.items())
        if not active_connections:
            return
        
        selected_connection = random_gen.choice(active_connections)[1]
        selected_connection.disable()

        new_node_id = len(self.node_genes) + 1
        new_node = random_gen.shuffle(node_classes)(new_node_id, NodeType.HIDDEN)

        in_to_new = (selected_connection.in_node, new_node_id)
        new_to_out = (new_node_id, selected_connection.out_node)
        in_to_new_innovation = innovator.next_innovation_number(in_to_new)
        new_to_out_innovation = innovator.next_innovation_number(out_to_new)

        in_to_new = ConnectionGene(*in_to_new, 1.0, True, in_to_new_innovation)
        new_to_out = ConnectionGene(*new_to_out, selected_connection.weight, True, new_to_out_innovation)

        self.node_genes.append(new_node)
        self.connection_genes.extend([in_to_new, new_to_out])

    def vizualize_genome(self, window_id, title):
        G = nx.Graph()
        G.add_nodes_from(range(len(self.node_genes.keys())))
        G.add_weighted_edges_from([(node[1].in_node, node[1].out_node, node[1].weight) for node in self.connection_genes.items()])

        color_map = []
        for node in G:
            node_type = self.node_genes[node]._type
            if node_type == NodeType.INPUT:
                color_map.append("red")
            elif node_type == NodeType.HIDDEN:
                color_map.append("green")
            else:
                color_map.append("blue")

        edge_width = [a[2]['weight'] for a in G.edges(data=True)]

        plt.figure(window_id, figsize=(6, 5))
        ax = plt.gca()
        ax.set_title(title)
        nx.draw_spring(G, node_color=color_map, with_labels=True, width=edge_width, ax=ax)


    @staticmethod
    def crossover(parent1, parent2, random_gen, DISABLED_GENE_INHERITING_CHANCE):
        child = Genome()

        for node in parent1.node_genes.values():
            child.add_node_gene(node)

        for parent1_conn in parent1.connection_genes.values():
            if parent1_conn.innovation_number in parent2.connection_genes.keys():
                parent2_conn = parent2.connection_genes[parent1_conn.innovation_number]
                child_conn_gene = deepcopy(parent1_conn) if random_gen.random() < 0.5 else deepcopy(parent2_conn)
                
                enabled = parent1_conn.is_active and parent2_conn.is_active
                if enabled and (random_gen.random() < DISABLED_GENE_INHERITING_CHANCE):
                    child_conn_gene.disable()
                
                child.add_connection_gene(child_conn_gene)
            else:
                child_conn_gene = deepcopy(parent1_conn)
                child.add_connection_gene(child_conn_gene)
        
        return child
    
    @staticmethod
    def generate_offspring(genomeA, genomeB=None):
        # If only one genome, just do mutations
        # If both genomes are present perform a crossover alongwith mutations
        pass
    
    @staticmethod
    def gene_type_counts(genomeA, genomeB):
        matching_genes = 0
        disjoint_genes = 0

        innovsA = sorted(genomeA.connectionn_genes.keys())
        innovsB = sorted(genomeB.connectionn_genes.keys())

        indA = len(innovsA)
        indB = len(innovsB)

        while indA >= 0 or indB >= 0:
            if innovsA[indA] == innovsB[indB]:
                indA -= 1
                indB -= 1
                matching_genes += 1
                innovsA.pop(0)
                innovsB.pop(0)
            elif innovsA[indA] < innovsB[indB]:
                indA -= 1
                disjoint_genes += 1
                innovsA.pop(0)
            else:
                indB -= 1
                disjoint_genes += 1
                innovsB.pop(0)

        excess_genes = len(innovsA) + len(innovsB)

        return matching_genes, disjoint_genes, excess_genes
    
    @staticmethod
    def average_weight_difference(genomeA, genomeB):
        weight_difference = 0.0
        gene_matches = 0

        innovsA = sorted(genomeA.connection_genes.keys())
        innovsB = sorted(genomeB.connection_genes.keys())

        max_ind = max(innovsA[len(innovsA) - 1], innovsB[len(innovsB) - 1])
        for ind in range(max_ind):
            if ind in innovsA and ind in innovsB:
                gene_matches += 1
                weight_difference += fabs(genomeA.connection_genes[ind].weight - genomeB.connection_genes[ind].weight)
        
        return weight_difference / gene_matches if gene_matches > 0 else 0

    @staticmethod
    def compatibility_distance(genomeA, genomeB, PARAM_C1, PARAM_C2, PARAM_C3):
        _, disjoint_genes, excess_genes = gene_type_counts(genomeA, genomeB)
        avg_weight_diff = average_weight_difference(genomeA, genomeB)

        return PARAM_C1 * excess_genes + PARAM_C2 * disjoint_genes + PARAM_C3 * avg_weight_diff
