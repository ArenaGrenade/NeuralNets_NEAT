#ifndef lint
#define lint long long int
#endif

class NodeGene {

    private:
    lint id;
    Type type;
    public:
    NodeGene(lint id, Type type) {
        this->id = id;
        this->type = type;
    }

    lint get_id()  {
        return this->id;
    }

    Type get_type() {
        return this->type;
    }
};