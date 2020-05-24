#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <time.h>
#include "Config.h"
#include "NodeGene.h"
#include "ConnectionGene.h"
#include "Counter.h"
#include "Genome.h"
#include "GraphExporter.h"

int main() {
	srand(time(0));
    Genome genome;
    InnovationCounter counter;
    GraphExporter exporter;
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(1.0, 0.25);
    genome.addNode(NodeGene(1, INPUT));
    genome.addNode(NodeGene(2, INPUT));
    genome.addNode(NodeGene(3, HIDDEN));
    genome.addNode(NodeGene(4, OUTPUT));
    genome.addConnection(ConnectionGene(1, 3, 1.0112412, true, counter.getNextInnovationNumber(1, 3)));
    genome.addConnection(ConnectionGene(2, 3, 1.090131, true, counter.getNextInnovationNumber(2, 3)));
    genome.addConnection(ConnectionGene(3, 4, 1.02469, true, counter.getNextInnovationNumber(3, 4)));

	/*char output_index_string[90] = { 0 };

	sprintf_s(output_index_string, "./TestGraphs/NodeMutations/%d.txt", 1);
	std::cout << "Exporting network number " << 1 << " at " << output_index_string << std::endl;
	exporter.Export(output_index_string, genome);
	memset(output_index_string, 0, 90);
	
	for (int i = 2; i < 4; i++) {
		genome.addNodeMutation(counter);
		sprintf_s(output_index_string, "./TestGraphs/NodeMutations/%d.txt", i);
		std::cout << "Exporting network number " << i << " at " << output_index_string << std::endl;
		exporter.Export(output_index_string, genome);
		memset(output_index_string, 0, 90);
	}*/

	genome.addConnectionMutation(counter);
	genome.addConnectionMutation(counter);
	genome.addConnectionMutation(counter);
}