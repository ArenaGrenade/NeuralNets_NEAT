#ifndef lint
#define lint long long int
#endif

class Genome {
private:
	lint node_counter;
	std::map<lint, NodeGene> node_genes;
	std::map<lint, ConnectionGene> connection_genes;
	std::vector<std::vector<lint>> graph;

public:

	Genome() {
		node_counter = 1;
	}

    void addNode(NodeGene gene) {
        this->node_genes.insert(std::make_pair(gene.getId(), gene));
		node_counter = std::max(node_counter, gene.getId());
    }

    void addConnection(ConnectionGene gene) {
        this->connection_genes.insert(std::make_pair(gene.getInnovation(), gene));
    }

	void addUniformPerturbation(std::default_random_engine perturbation_generator,std::normal_distribution<double> uniform_perturbator) {
		for (auto connection : this->connection_genes) {
			double perturbed_value = connection.second.getWeight() + uniform_perturbator(perturbation_generator);
			if (perturbed_value < 1.0 && perturbed_value > -1.0) {
				connection.second.setWeight(perturbed_value);
			}
		}
	}

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

	void addConnectionMutation(InnovationCounter &counter) {
		this->graph.clear();
		this->graph = std::vector<std::vector<lint>>(this->getNumberOfNodes());
		for (std::map<lint, ConnectionGene>::iterator i = this->connection_genes.begin(); i != this->connection_genes.end(); i++)
			this->graph[i->second.getInNode() - 1].push_back(i->second.getOutNode() - 1);

		std::vector<std::pair<lint, lint>> reverse_connections;

		lint u = 0;

		for (auto& adjl : graph) {
			sort(adjl.begin(), adjl.end());
			lint v = 0;
			for (auto end_vertex : adjl) {
				if (end_vertex != v) reverse_connections.push_back(std::make_pair(u, v));
				else v++;
			}
			while (v < adjl.size()) {
				reverse_connections.push_back(std::make_pair(u, v++));
				//std::cout << "reverse connections are: " << u << "to" << v << std::endl;
			}
			u++;
		}

		auto random_non_connection = reverse_connections.at(rand() % reverse_connections.size());

		ConnectionGene random_gene_added(random_non_connection.first + 1, random_non_connection.second + 1, (rand() % 1000) / (double) 1000, true, counter.getNextInnovationNumber(random_non_connection.first + 1, random_non_connection.second + 1));

		std::cout << random_gene_added.getInNode() << " to " << random_gene_added.getOutNode() << std::endl;

		this->addConnection(random_gene_added);

		std::cout << "New connection is: " << random_non_connection.first + 1 << " " << random_non_connection.second + 1 << "\n";
	}

    std::map<lint, NodeGene> getNodeGenes() {
        return this->node_genes;
    }

    std::map<lint, ConnectionGene> getConnectionGenes() {
        return this->connection_genes;
    }

    lint getNumberOfNodes() {
        return this->node_genes.size();
    }

    lint getNumberOfConnections() {
        return this->connection_genes.size();
    }
};