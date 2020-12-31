#pragma once
#include "Genome.h"

#pragma once
#include "FitnessGenome.h"

#pragma once
#include "FitnessGenomeComparator.h"

#pragma once
#include "Counter.h"

#ifndef lint
#define lint long long int
#endif

class Evaluator {
	private:
		// insert some private variables here
		std::vector<Genome> genomes;
		std::vector<Genome> nextGeneration;
		std::vector<FitnessGenome> evaluatedGenomes;
		FitnessGenome fittestGenome;
		std::vector<FitnessGenome> lastGenerationResults;
		Counter nodeInnovation;
		Counter connectionInnovation;
	public:
		// insert some public variables here
		Evaluator(Genome initialGenome) {
			
		}
};

class Species {
public:
	Genome mascot;
	std::vector<Genome> members;
};
