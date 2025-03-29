//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <memory>


namespace Sim {
    class Node {
        std::unique_ptr<bool[]> input;
        std::unique_ptr<bool[]> output;
        std::unique_ptr<Node *[]> references;

    public:
        Node(bool pure, unsigned int outputs);

        bool pure;

        bool getInput(unsigned int index);

        bool getOutput(unsigned int index);

        void setInput(unsigned int index, bool value);
    };
};


#endif //NODE_H
