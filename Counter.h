#ifndef lint
#define lint long long int
#endif

class InnovationCounter {
    /*
    ----InnovationCounter Class----

     This class defines the counter that keeps track of innovations of the gene. It has the following parameters.

    * present_count - long long int: The highest innovation number in consideration.
    * innovation_list - std::map<std::pair<long long int, long long int>, long long int>: Keeps track of the connections and their corresponding innovation number.

     Getter methods
    long long int getNextInnovationNumber(long long int in_node, long long int out_node);

     Setter methods
    */

  private:
    lint present_count;
	std::map<std::pair<lint, lint>, lint> innovation_list;
  public:
    //----Constructors----//
    
    // The base constructor for this class
    InnovationCounter() {
        present_count = 1;
    }

    //----Getters----//

    // This function takes the in_node and out_node of a connection and checks if a node with similar properties already exists. \
    // If it exists, the present innovation number is returned, else a new innovation number is created and that is returned.
    // in_node - long long int: The starting node ID of the connection
    // out_node - long long int: The ending node ID of the connection
    lint getNextInnovationNumber(lint in_node, lint out_node) {
		std::map<std::pair<lint, lint>, lint>::iterator match_find = this->innovation_list.find(std::make_pair(in_node, out_node));
		if (match_find == this->innovation_list.end()) {
			this->innovation_list.insert(std::make_pair(std::make_pair(in_node, out_node), present_count));
			//std::cout << "no match found with innovation number so returning new innovation number\n";
			return present_count++;
		}
		//std::cout << "match has been found with this pair: " << (*match_find).first.first << ":" << (*match_find).first.second << " " << in_node << ":" << out_node<<"\n";
        return (*match_find).second;
    }
};