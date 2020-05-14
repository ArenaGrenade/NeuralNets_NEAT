#ifndef lint
#define lint long long int
#endif

class Genome {
    private:
    std::map<lint, NodeGene> node_genes;
    std::map<lint, ConnectionGene> connection_genes;

    public:
    void addNode(NodeGene gene) {
        this->node_genes.insert(std::make_pair(gene.getId(), gene));
    }

    void addConnection(ConnectionGene gene) {
        this->connection_genes.insert(std::make_pair(gene.getInnovation(), gene));
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