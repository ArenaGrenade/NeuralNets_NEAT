#ifndef lint
#define lint long long int
#endif

class Genome {
    private:
    std::map<lint, NodeGene> NodeGenes;
    std::map<lint, ConnectionGene> ConnectionGenes;
    public:
    Genome() {
        this->NodeGenes = new std::map<lint, NodeGene>;

    }
};