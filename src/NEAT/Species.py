class Species:
    def __init__(self, representor_id, fitness):
        self.representor = representor_id
        self.count_genomes = 1
        self.total_fitness = fitness
        self.genomes = [representor_id]
    
    def add_genome(self, genome_id, fitness):
        if genome_id in self.genomes:
            return
        
        self.genomes.append(genome_id)
        self.total_fitness += fitness
        self.count_genomes += 1
