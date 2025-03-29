//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <memory>
#include <vector>


namespace Sim {
    class Node;

    class Pin {
        bool value = false;
        bool dirty = false;

    public:
        unsigned int index;

        explicit Pin(unsigned int index);

        [[nodiscard]] bool getValue() const;

        void setValue(bool value);

        [[nodiscard]] bool pollDirty();
    };

    class Node {
    public:
        std::vector<std::shared_ptr<Pin> > outputPins;
        std::vector<std::weak_ptr<Pin> > inputPins;

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
        explicit AndNode(char inputs);

        ~AndNode() override = default;

        void update() override;
    };

    class NotNode final : public Node {
    public:
        NotNode();

        ~NotNode() override = default;

        void update() override;
    };

    class OrNode final : public Node {
    public:
        explicit OrNode(char inputs);

        ~OrNode() override = default;

        void update() override;
    };
};


#endif //NODE_H
