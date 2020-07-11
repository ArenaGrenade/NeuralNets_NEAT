#ifndef lint
#define lint long long int
#endif

class NodeGene {
    /*
    ----NodeGene Class----

     This class describe a Node in the network. Describes the following parameters of it.
    * id - long long int: The ID given to the current node.
    * type - enum Type: Describes the type of the node (Input, Hiddden, Output)

     Getter Methods
    long long int getId();
    enum Type getType();

     Setter Methods
    */

  private:
    lint id;
    Type type;

  public:
    //----Constructors----//

    // Base constructor
    NodeGene(lint id, Type type) {
        this->id = id;
        this->type = type;
    }


    //----Getters----//

    // Returns the ID of this node
    lint getId()  {
        return this->id;
    }

    // Returns the node type of this node
    Type getType() {
        return this->type;
    }
};