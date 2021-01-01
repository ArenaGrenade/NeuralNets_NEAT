from Genome import Genome
from FitnessStructure import FitnessStructure
from random import Random

class Evaluator:
    def __init__(self, fitness_function, innovator):
        self.current_generation = []
        self.previous_generation = []
        self.random = Random()

        self.fitness_function = fitness_function
        self.innovator = innovator

    def initialize_population(self, population=None, size=None):
        if population is not None:
            self.previous_generation = list(map(lambda x: FitnessStructure(x, 0.0), population))

    def evaluate_generation(self):
        # TODO: Have to implement speciation
        #! useless constants to go into config
        SELECTION_RATIO = 0.1
        DISABLED_GENE_INHERITING_CHANCE = 0.5
        #!
        # Score each genome
        for fit_genome in self.previous_generation:
            fit_genome.fitness = self.fitness_function(fit_genome.genome)
        
        # Update stats like best genome and all
        self.previous_generation = sorted(self.previous_generation, key=lambda fit_genome: fit_genome.fitness, reverse=True)
        self.best_individual = self.previous_generation[0]

        # Perform selection of the needed ration
        select_count = max(3, int(SELECTION_RATIO * len(self.previous_generation)))
        self.current_generation = parents = self.previous_generation[:select_count]

        # Perform random mating and also perform mutation to generate next generation
        num_generated = 0
        while num_generated < len(self.previous_generation) - select_count:
            parent1 = self.random.choice(parents)
            parent2 = self.random.choice([parent for parent in parents if parent != parent1])
            child = Genome.crossover(parent1.genome, parent2.genome, self.random, DISABLED_GENE_INHERITING_CHANCE)

            child.add_connection_mutation(self.random, self.innovator)

            self.current_generation.append(FitnessStructure(child, 0.0))
            num_generated += 1

        # Shift all data lists and perform any needed checkpoint stores or run provided call back function
        self.previous_generation = self.current_generation
