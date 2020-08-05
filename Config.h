#ifndef lint
#define lint long long int
#endif

// This enum is the base type for deciding the kind of node in the network.
// Any node is of three types, namely:
// INPUT - A node where the input is provided externally
// HIDDEN - This is a node where the input as well as the output are not visible externally
// OUTPUT - The node where any form of output is taken from.
enum Type {INPUT, HIDDEN, OUTPUT};

// The path to where any graphs created will be saved.
char GRAPH_SAVE_PATH[] = "C:/Users/rohan/Documents/Coding/C++/NeuralNets_NEAT/TestGraphs/NodeMutations/";

// The connection mutation requires some pruning. This value denotes the maximum number of steps to perform before stopping the mutation.
lint MAX_CONNECTION_MUTATION_TRIES = 20;