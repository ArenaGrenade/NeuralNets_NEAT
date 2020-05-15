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

    lint getId()  {
        return this->id;
    }

    Type getType() {
        return this->type;
    }
};