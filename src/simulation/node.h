//
// Created by felix on 29.03.25.
//

#ifndef NODE_H
#define NODE_H
#include <memory>
#include <unordered_set>
#include <vector>


namespace Sim {
    class node;

    class Pin {
        bool value = false;
        bool dirty = false;

    public:
        std::unordered_set<node *> nodes = {};
        unsigned int index;

        explicit Pin(unsigned int index);

        [[nodiscard]] bool getValue() const;

        void setValue(bool value);

        [[nodiscard]] bool pollDirty();
    };

    class node {
    public:
        std::vector<std::unique_ptr<Pin> > outputPins;
        std::vector<Pin *> inputPins;

        explicit node(bool pure, unsigned int inputs, unsigned int outputs);

        virtual ~node() = default;

        bool pure;

        [[nodiscard]] bool getInput(unsigned int index) const;

        [[nodiscard]] bool getOutput(unsigned int index) const;

        virtual void update() = 0;

    protected:
        void setOutput(unsigned int index, bool value) const;
    };

    class AndNode final : public node {
    public:
        explicit AndNode(char inputs);

        ~AndNode() override = default;

        void update() override;
    };

    class NotNode final : public node {
    public:
        NotNode();

        ~NotNode() override = default;

        void update() override;
    };

    class OrNode final : public node {
    public:
        explicit OrNode(char inputs);

        ~OrNode() override = default;

        void update() override;
    };
};


#endif //NODE_H
