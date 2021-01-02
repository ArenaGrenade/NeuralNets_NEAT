from Genome import Genome
from random import Random
from Species import Species
from functools import reduce
import json
from dotted_dict import DottedDict

class Evaluator:
    def __init__(self, fitness_function, innovator, config_path):
        self.current_generation = {}
        self.previous_generation = {}
        self.random = Random()

        self.fitness_function = fitness_function
        self.innovator = innovator

        self.config = self.parse_config(config_path)

    def parse_config(self, config_path):
        with open(config_path, 'r') as config_file:
            parsed_config = json.load(config_file)
        with open("default_config.json", 'r') as default_config_file:
            default_config = json.load(default_config_file)
        config_dict = {**default_config, **parsed_config}
        return DottedDict(config_dict)

    def initialize_population(self, population=None, size=None):
        if population is not None:
            self.previous_generation = population

    def evolve_generation(self):
        # Score each genome
        for fit_genome in self.previous_generation:
            fit_genome.fitness = self.fitness_function(fit_genome)
        
        # Update stats like best genome and all
        self.previous_generation = sorted(self.previous_generation, key=lambda fit_genome: fit_genome.fitness, reverse=True)
        self.best_individual = self.previous_generation[0]

        # Perform selection of the needed ration
        select_count = max(3, int(self.config.SELECTION_RATIO * len(self.previous_generation)))
        self.current_generation = parents = self.previous_generation[:select_count]

        # Perform random mating and also perform mutation to generate next generation
        num_generated = 0
        while num_generated < len(self.previous_generation) - select_count:
            parent1 = self.random.choice(parents)
            parent2 = self.random.choice([parent for parent in parents if parent != parent1])
            child = Genome.crossover(parent1, parent2, self.random, self.config.DISABLED_GENE_INHERITING_CHANCE)

            child.add_connection_mutation(self.random, self.innovator)

            self.current_generation.append(child)
            num_generated += 1

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
                if Genome.compatibility_distance(self.previous_generation[species.representor_id], genome) <= self.config.COMPATIBILITY_THRESHOLD:
                    species.add_genome(genome.id, genome.fitness)
                    genome_added = True
                    break
            if not genome_added:
                species_list.append(Species(genome.id, genome.fitness))
            species_list.sort(key=lambda species: species.total_fitness, reverse=True)
        # Recalculate the adjusted fitness based on species
        for species in species_list:
            for genome_id in species.genomes:
                self.previous_generation[genome_id].fitness /= species.count_organisms
        # Kill off low performing individuals
        fitness_sorted_genome_ids = [genome[0] for genome in sorted(self.previous_generation.items(), key=lambda genome: genome[1].fitness, reverse=True)]
        parents = fitness_sorted_genome_ids[:int(self.config.SELECTION_RATIO * self.config.POPULATION_SIZE)]
        for species in species_list:
            for genome_id in species.genomes:
                if genome_id not in parents:
                    species.genomes.remove(genome_id)
        # Allocate child count per-species
        allocation_ratio = POPULATION_SIZE / reduce(lambda a, b: a.total_fitness + b.total_fitness, species_list)
        per_species_allocation = [(index, species.total_fitness * allocation_ratio) for index, species in enumerate(species_list)]
        # Create offspring to fill up remaining space by random mating and mutations based on species size
        for (index, child_count) in enumerate(per_species_allocation):
            for _ in range(child_count):
                parent1 = self.previous_generation[self.random.choice(species_list[index].genomes)]
                parent2 = self.previous_generation[self.random.choice([genome for genome in species_list[index].genomes if genome != parent1])]
                child = Genome.generate_offspring(parent1, parent2)
                self.current_generation[child.id] = child
        
        present_population_size = len(self.current_generation)
        while present_size <= POPULATION_SIZE:
            parent1 = self.previous_generation[self.random.choice(parents)]
            parent2 = self.previous_generation[self.random.choice([parent for parent in parents if parent != parent1])]
            child = Genome.generate_offspring(parent1, parent2)
            self.current_generation[child] = child
            present_population_size += 1
        # Update all lists and perform logging
        self.previous_generation = self.current_generation
