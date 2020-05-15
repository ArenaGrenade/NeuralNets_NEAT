#ifndef lint
#define lint long long int
#endif

class Genome {
private:
	lint node_counter;
	std::map<lint, NodeGene> node_genes;
	std::map<lint, ConnectionGene> connection_genes;

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