from Genome import Genome
from random import Random
from Species import Species
from functools import reduce
import json
from dotted_dict import DottedDict
from NodeType import NodeType
from ConnectionGene import ConnectionGene
from pprint import pprint

class Evaluator:
    def __init__(self, fitness_function, innovator, node_classes, config_path=None):
        self.current_generation = {}
        self.previous_generation = {}
        self.random = Random()
        self.max_id = 0

        self.fitness_function = fitness_function
        self.innovator = innovator
        self.node_classes = node_classes

        if config_path:
            self.config = self.parse_config(config_path)
        else:
            self.config = self.parse_config()

    def parse_config(self, config_path=None):
        if config_path:
            with open(config_path, 'r') as config_file:
                parsed_config = json.load(config_file)
        else:
            parsed_config = {}
        with open("default_config.json", 'r') as default_config_file:
            default_config = json.load(default_config_file)
        config_dict = {**default_config, **parsed_config}
        return DottedDict(config_dict)

    def initialize_population(self, population=None, size=None):
        if population is not None:
            self.previous_generation = population
            self.max_id = len(population.keys())
        elif size is not None:
            self.max_id = self.config.POPULATION_SIZE
            pop_size = self.config.POPULATION_SIZE
            input_size = size[0]
            output_size = size[1]

            for genome_id in range(pop_size):
                genome = Genome(genome_id)

                node_id = 0
                # Input Nodes
                for _ in range(input_size):
                    node_gene = self.random.choice(self.node_classes)(node_id, NodeType.INPUT)
                    genome.add_node_gene(node_gene)
                    node_id += 1
                
                # Output Nodes
                for _ in range(output_size):
                    node_gene = self.random.choice(self.node_classes)(node_id, NodeType.OUTPUT)
                    genome.add_node_gene(node_gene)
                    node_id += 1
                
                # Connect each input to every other output
                for in_id in range(input_size):
                    for out_id in range(output_size):
                        connection = ConnectionGene(in_id, out_id + input_size, 1.0, True, self.innovator.next_innovation_number((in_id, out_id)))
                        genome.add_connection_gene(connection)

                self.previous_generation[genome_id] = genome
        else:
            raise ValueError("Invalid Parameters")

    def evolve_generation(self):
        # Score each genome
        for fit_genome in self.previous_generation.values():
            fit_genome.fitness = self.fitness_function(fit_genome)
        
        # Update stats like best genome and all
        fitness_sorted_genome_ids = [genome[0] for genome in sorted(self.previous_generation.items(), key=lambda genome: genome[1].fitness, reverse=True)]
        self.best_individual = self.previous_generation[fitness_sorted_genome_ids[0]]

        # Perform selection of the needed ration
        select_count = int(self.config.SELECTION_RATIO * self.config.POPULATION_SIZE)
        parents = fitness_sorted_genome_ids[:select_count]

        # Perform random mating and also perform mutation to generate next generation
        for _ in range(self.config.POPULATION_SIZE):
            parent1 = self.random.choice(parents)
            parent2 = self.random.choice([parent for parent in parents if parent != parent1])
            child = Genome.generate_offspring(self.previous_generation[parent1], self.max_id, self.random, self.node_classes, self.innovator, self.config, self.previous_generation[parent2])
            self.current_generation[child.id] = child
            self.max_id += 1

        # Shift all data lists and perform any needed checkpoint stores or run provided call back function
        self.previous_generation = self.current_generation

    def evolve_generation_speciated(self):
        # Clean out our previous species array
        species_list = []
        self.current_generation = {}
        # Find the fitness of all individuals of the previous generation,
        # And also assign a species to all of them
        for genome in self.previous_generation.values():
            genome.fitness = self.fitness_function(genome)
            genome_added = False
            for species in species_list:
                cd = Genome.compatibility_distance(self.previous_generation[species.representor], genome, self.config.PARAM_C1, self.config.PARAM_C2, self.config.PARAM_C3)
                if cd <= self.config.COMPATIBILITY_THRESHOLD:
                    species.add_genome(genome.id, genome.fitness)
                    genome_added = True
                    break
            if not genome_added:
                species_list.append(Species(genome.id, genome.fitness))
            species_list.sort(key=lambda species: species.total_fitness, reverse=True)
        # Recalculate the adjusted fitness based on species
        for species in species_list:
            for genome_id in species.genomes:
                self.previous_generation[genome_id].fitness /= species.count_genomes
        # Kill off low performing individuals
        fitness_sorted_genome_ids = [genome[0] for genome in sorted(self.previous_generation.items(), key=lambda genome: genome[1].fitness, reverse=True)]
        parents = fitness_sorted_genome_ids[:int(self.config.SELECTION_RATIO * self.config.POPULATION_SIZE)]
        for species in species_list:
            for genome_id in species.genomes:
                if genome_id not in parents:
                    species.genomes.remove(genome_id)
        species = [species for species in species_list if species]
        
        # Allocate child count per-species and create children
        total_fitness = reduce(lambda a, b: a + b, [s.total_fitness for s in species_list])
        if total_fitness > 0.0:
            allocation_ratio = self.config.POPULATION_SIZE / total_fitness
            per_species_allocation = [(index, int(species.total_fitness * allocation_ratio)) for index, species in enumerate(species_list)]
            # Create offspring to fill up remaining space by random mating and mutations based on species size
            for index, child_count in per_species_allocation:
                for _ in range(child_count):
                    parent1 = self.previous_generation[self.random.choice(species_list[index].genomes)]
                    parent2 = self.previous_generation[self.random.choice([genome for genome in species_list[index].genomes if genome != parent1])]
                    child = Genome.generate_offspring(parent1, self.max_id, self.random, self.node_classes, self.innovator, self.config, parent2)
                    self.current_generation[child.id] = child
        
        present_population_size = len(self.current_generation)
        while present_population_size <= self.config.POPULATION_SIZE:
            parent = self.previous_generation[self.random.choice(parents)]
            child = Genome.generate_offspring(parent, self.max_id, self.random, self.node_classes, self.innovator, self.config)
            self.current_generation[child.id] = child
            present_population_size += 1
            self.max_id += 1
        # Update all lists and perform logging
        self.previous_generation = self.current_generation
