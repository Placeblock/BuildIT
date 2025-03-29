//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <memory>
#include <vector>


namespace Sim {
    class Node;

    struct Pin {
        Node *node;
        bool value = false;

        explicit Pin(Node *node);
    };

    class Node {
    protected:
        std::vector<std::shared_ptr<Pin> > outputPins;
        std::vector<std::weak_ptr<Pin> > inputPins;

    public:
        explicit Node(bool pure, unsigned int inputs, unsigned int outputs);

        virtual ~Node() = default;

        bool pure;

        [[nodiscard]] bool getInput(unsigned int index) const;

        [[nodiscard]] bool getOutput(unsigned int index) const;

        virtual void update() = 0;

    protected:
        void setOutput(unsigned int index, bool value) const;
    };

    class AndNode final : public Node {
    public:
        AndNode(bool pure, char inputs);

        void update() override;
    };

    class NotNode final : public Node {
    public:
        explicit NotNode(bool pure);

        void update() override;
    };

    class OrNode final : public Node {
    public:
        explicit OrNode(bool pure, char inputs);

        void update() override;
    };
};


#endif //NODE_H
