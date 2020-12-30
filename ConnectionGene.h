#ifndef lint
#define lint long long int
#endif


class ConnectionGene {
    /*
    ----ConnectionGene Class----

     This class defines a connection between nodes in the network. It maintains the following parameters about the connection.

    * in_node - long long int: The ID of the node that is at the start of this connection.
    * out_node - long long int: The ID of the node that is at the end of this connection.
    * weight - double: The weight of this conenction.
    * enabled - boolean: Enables or disable information flow through this connection.
    * innovation_number - long long int: The innovation number for this connection gene.

     Setter methods
    void setWeight(long long int);
    void disableConnection();

     Getter methods
    long long int getInNode();
    long long int getOutNode();
    long long int getInnovation();
    double getWeight();
    bool isEnabled();
    */

private:
    lint in_node;
    lint out_node;
    double weight;
    bool enabled;
    lint innovation_number;

public:
    //----Constructors----//

    // The base constructor for the class.
    ConnectionGene(lint in_node, lint out_node, double weight, bool enabled, lint innovation_number) {
        this->in_node = in_node;
        this->out_node = out_node;
        this->weight = weight;
        this->enabled = enabled;
        this->innovation_number = innovation_number;
    }

	// The copy constructor
    // This creates a copy of the given ConnectionGene instance
	ConnectionGene(const ConnectionGene &connection2) {
		this->in_node = connection2.in_node;
		this->out_node = connection2.out_node;
		this->weight = connection2.weight;
		this->enabled = connection2.enabled;
		this->innovation_number = connection2.innovation_number;
	}


    //----Getters----//
    
    // Returns the in_node value
    lint getInNode() {
        return this->in_node;
    }
    
    // Returns the out_node value
    lint getOutNode() {
        return this->out_node;
    }
    
    // Returns the innovation number of this connection
    lint getInnovation() {
        return this->innovation_number;
    }
    
    // Returns the weight of this connection
    double getWeight() {
        return this->weight;
    }
    
    // Returns the boolean stating whether this conenction is active or not
	bool isEnabled() {
		return this->enabled;
	}


    //----Setters----//

    // Sets the weight of this conenction to the ggiven value
    void setWeight(double weight) {
        this->weight = weight;
    }

    // Disables the current connection
    void disableConnection() {
        this->enabled = false;
    }
};