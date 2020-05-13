#ifndef lint
#define lint long long int
#endif

class ConnectionGene {
    private:
    lint in_node;
    lint out_node;
    double weight;
    bool enabled;
    lint innovation_number;

    public:
    //basic constructor
    ConnectionGene(lint in_node, lint out_node, double weight, bool enabled, lint innovation_number) {
        this->in_node = in_node;
        this->out_node = out_node;
        this->weight = weight;
        this->enabled = enabled;
        this->innovation_number = innovation_number;
    }

    //used to change weight durng mutations
    void setWeight(double weight) {
        this->weight = weight;
    }

    //again changes only during mutations
    void disableConnection() {
        this->enabled = false;
    }

    //standard get functions
    lint getInNode() {
        return this->in_node;
    }

    lint getOutNode() {
        return this->out_node;
    }

    lint getInnovation() {
        return this->innovation_number;
    }
};