#ifndef lint
#define lint long long int
#endif

class NodeGene {
    /**
     * @brief This class describes a Node in the network.
     * 
     */

    private:
        lint id;
        Type type;

    public:
        /**
         * @brief Construct a new Node Gene object
         * 
         * @param id The ID of the new Node
         * @param type THe type of the node
         */
        NodeGene(lint id, Type type) {
            this->id = id;
            this->type = type;
        }

        /**
         * @brief Create a copy of a Node Gene object
         * 
         * @param node2 Node to be copied
         */
        NodeGene(const NodeGene &node2) {
            this->type = node2.type;
            this->id = node2.id;
        }

        /**
         * @brief Get the Id of the Node object
         * 
         * @return lint The ID of the Node object
         */
        lint getId()  {
            return this->id;
        }

        /**
         * @brief Get the Type of the Node object
         * 
         * @return Type The type of the Node object
         */
        Type getType() {
            return this->type;
        }
};