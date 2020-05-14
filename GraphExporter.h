#ifndef lint
#define lint long long int
#endif

class GraphExporter {
    private:
    std::vector<std::vector<lint>> graph;
    std::ofstream fout;
    lint graph_size;
    std::map<lint, ConnectionGene> present_connections;
    
    public:
    GraphExporter() {
        graph_size = 1;
    }

    void Export(const char* path, Genome g) {
		this->graph = std::vector<std::vector<lint>>(g.getNumberOfNodes());
        this->present_connections = g.getConnectionGenes();
        this->fout.open(path);
        for(std::map<lint, ConnectionGene>::iterator i = this->present_connections.begin(); i != this->present_connections.end(); i++)
            this->graph[i->second.getInNode()-1].push_back(i->second.getOutNode()-1);
        
		for (lint i = 0; i < this->graph.size(); i++) {
			this->fout << i << " ";
			for (lint j = 0; j < this->graph[i].size(); j++) {
				this->fout << " " << this->graph[i][j];
			}
			this->fout << "\n";
		}
    }
};