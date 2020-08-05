#include <list>
#ifndef lint
#define lint long long int
#endif

class Genome {
	/*
	----Genome Class----

	 This class defines the genome of any individual. It is the core component of the Genetic Algorithm. It defines the following variables.

	* node_counter - long long int: The number of nodes in the current genome.
	* node_genes - std::map<long long int, NodeGene>: The C++ map that contains a list of NodeGene and their IDs
	* connection_genes - std::map<long long int, ConnectionGene>: The C++ map that consists of the Innovation number and the ConnectionGene
	* graph - std::vector<std::vector<long long int>>: A 2-D array that will represent a graph in adjacency list format in some of the intermediate steps.
	*/

  private:
	lint node_counter;
	std::map<lint, NodeGene> node_genes;
	std::map<lint, ConnectionGene> connection_genes;
	std::vector<std::vector<lint>> graph;

  public:
	//----Constructors----//

	// Base constructor for the class
	Genome() {
		node_counter = 1; // node_counter is 1-indexed
	}

	//----Getters----//

	// returns the map of nodes of this genome
	std::map<lint, NodeGene> getNodeGenes() {
		return this->node_genes;
	}

	// returns a map of connections of the genome
	std::map<lint, ConnectionGene> getConnectionGenes() {
		return this->connection_genes;
	}

	// returns the size of network
	lint getNumberOfNodes() {
		return this->node_genes.size();
	}

	// returns the number of connections in the network
	lint getNumberOfConnections() {
		return this->connection_genes.size();
	}

	//----Add a new gene type----//

	// Adds a new node to the current genome, given a NodeGene object
    void addNode(NodeGene gene) {
        this->node_genes.insert(std::make_pair(gene.getId(), gene));
		node_counter = std::max(node_counter, gene.getId());
    }

	// Adds a new connection to the genome, given a ConnectionGene object
    void addConnection(ConnectionGene gene) {
        this->connection_genes.insert(std::make_pair(gene.getInnovation(), gene));
    }

	//----Mutations----//

	// Adds a uniform perturbution to the weights of every node in the network represented by this genome
	void addUniformPerturbation(std::default_random_engine perturbation_generator,std::normal_distribution<double> uniform_perturbator) {
		for (auto connection : this->connection_genes) {
			double perturbed_value = connection.second.getWeight() + uniform_perturbator(perturbation_generator);
			if (perturbed_value < 1.0 && perturbed_value > -1.0) {
				connection.second.setWeight(perturbed_value);
			}
		}
	}

	// Adds a random node to the network in the middle of a randomly chosen connection
	void addNodeMutation(InnovationCounter &counter) {
		auto random_connection_gene = this->connection_genes.begin();
		std::advance(random_connection_gene, rand() % this->connection_genes.size());
		ConnectionGene mod_conn((*random_connection_gene).second);
		std::cout << "Connection mutated is: " << mod_conn.getInNode() << " -> " << mod_conn.getOutNode() << "\n";

		mod_conn.disableConnection();

		NodeGene new_node(++node_counter, HIDDEN);
		std::cout << "The new node added is: " << new_node.getId() << "\n";
		ConnectionGene in_to_new(mod_conn.getInNode(), new_node.getId(), 1.0, true, counter.getNextInnovationNumber(mod_conn.getInNode(), new_node.getId()));
		ConnectionGene new_to_out(new_node.getId(), mod_conn.getOutNode(), mod_conn.getWeight(), true, counter.getNextInnovationNumber(new_node.getId(), mod_conn.getOutNode()));
		std::cout << "The new connections made are: " << in_to_new.getInNode() << " -> " << in_to_new.getOutNode() << " and " << new_to_out.getInNode() << "->" << new_to_out.getOutNode() << "\n";
		std::cout << "The innovation numbers in order are: " << in_to_new.getInnovation() << " " << new_to_out.getInnovation() << "\n";

		this->node_genes.insert(std::make_pair(new_node.getId(), new_node));
		this->connection_genes.insert(std::make_pair(in_to_new.getInnovation(), in_to_new));
		this->connection_genes.insert(std::make_pair(new_to_out.getInnovation(), new_to_out));
	}

	// Adds a new connection with a random weight between any two previously un-connected nodes
	void addConnectionMutation(InnovationCounter &counter) {
		int tries = 0;
		bool success = false;

		while (tries < MAX_CONNECTION_MUTATION_TRIES && success == false) {
			tries++;

			auto random_node_gene = this->node_genes.begin();
			std::advance(random_node_gene, rand() % this->node_genes.size());
			NodeGene nodeA = (*random_node_gene).second;

			random_node_gene = this->node_genes.begin();
			std::advance(random_node_gene, rand() % this->node_genes.size());
			NodeGene nodeB = (*random_node_gene).second;

			bool is_reversed = false;
			if (nodeA.getType() == INPUT && nodeB.getType() == INPUT || 
				nodeA.getType() == OUTPUT && nodeB.getType() == HIDDEN || 
				nodeA.getType() == OUTPUT && nodeB.getType() == INPUT) {
				is_reversed = true;
			}

			if (is_reversed) {
				NodeGene node_swap_temp = nodeA;
				nodeA = nodeB;
				nodeB = node_swap_temp;
			}

			bool is_connection_possible = true;
			if (nodeA.getType() == INPUT && nodeB.getType() == INPUT || 
				nodeA.getType() == OUTPUT && nodeB.getType() == OUTPUT || 
				nodeA.getId() == nodeB.getId()) {
				is_connection_possible = false;
			}

			std::list<lint> needsChecking;
			std::list<lint> nodeIDs;
			for (auto connection : this->connection_genes) {
				if (connection.second.getInNode() == nodeB.getId()) {
					nodeIDs.push_back(connection.second.getOutNode());
					needsChecking.push_back(connection.second.getOutNode());
				}
			}

			while (needsChecking.size() != 0) {
				lint nodeID = needsChecking.front();
				for (auto connection : this->connection_genes) {
					if (connection.second.getInNode() == nodeID) {
						nodeIDs.push_back(connection.second.getOutNode());
						needsChecking.push_back(connection.second.getOutNode());
					}
				}
				needsChecking.pop_front();
			}

			for (lint ID : nodeIDs) {
				if (ID == nodeA.getId()) {
					is_connection_possible = false;
				}
			}

			bool connection_exists = false;
			for (auto connection : this->connection_genes) {
				if (connection.second.getInNode() == nodeA.getId() && connection.second.getOutNode() == nodeB.getId() || 
					connection.second.getInNode() == nodeB.getId() && connection.second.getOutNode() == nodeA.getId()) {
					connection_exists = true;
					break;
				}
			}

			if (!connection_exists && is_connection_possible) {
				double random_weight = rand() % INT_MAX;
				ConnectionGene new_connection(nodeA.getId(), nodeB.getId(), random_weight, true, counter.getNextInnovationNumber(nodeA.getId(), nodeB.getId()));
				this->connection_genes.insert(std::make_pair(new_connection.getInnovation(), new_connection));
				success = true;
			}

		}
		if (!success) {
			std::cout << "Tried, but no connections was added" << std::endl;
		}
		else {
			std::cout << "Created Connection" << std::endl;
		}
	}
};