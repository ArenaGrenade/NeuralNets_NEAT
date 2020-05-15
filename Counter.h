#ifndef lint
#define lint long long int
#endif

class InnovationCounter {
    private:
    lint present_count;
	std::map<std::pair<lint, lint>, lint> innovation_list;
    public:
    InnovationCounter() {
        //the counter is 1-indexed.
        present_count = 1;
    }

    //this is the main function of the counter - returns the value and increments the counter.
    lint getNextInnovationNumber(lint in_node, lint out_node) {
		std::map<std::pair<lint, lint>, lint>::iterator match_find = this->innovation_list.find(std::make_pair(in_node, out_node));
		if (match_find == this->innovation_list.end()) {
			this->innovation_list.insert(std::make_pair(std::make_pair(in_node, out_node), present_count));
			std::cout << "no match found with innovation number so returning new innovation number\n";
			return present_count++;
		}
		std::cout << "match has been foound with this pair: " << (*match_find).first.first << ":" << (*match_find).first.second << " " << in_node << ":" << out_node<<"\n";
        return (*match_find).second;
    }
};