#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "Config.h"
#include "NodeGene.h"
#include "ConnectionGene.h"
#include "Counter.h"
#include "Genome.h"
#include "GraphExporter.h"

int main() {
    Genome genome;
    InnovationCounter counter;
    GraphExporter exporter;
    genome.addNode(NodeGene(1, INPUT));
    genome.addNode(NodeGene(2, INPUT));
    genome.addNode(NodeGene(3, HIDDEN));
    genome.addNode(NodeGene(4, OUTPUT));
    genome.addConnection(ConnectionGene(1, 3, 1.0112412, true, counter.getInnovationNumber()));
    genome.addConnection(ConnectionGene(2, 3, 1.090131, true, counter.getInnovationNumber()));
    genome.addConnection(ConnectionGene(3, 4, 1.02469, true, counter.getInnovationNumber()));
    exporter.Export("graphs.txt", genome);
}