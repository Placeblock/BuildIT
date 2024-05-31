#ifndef TREE_H
#define TREE_H

#include <cstdint>
#include <vector>
#include <map>

struct PinReference {
    struct Gate* gate;
    uint8_t index;
};

struct Child {
    PinReference* reference;
    uint8_t output;
};

struct Parent {
    PinReference* reference;
    uint8_t input;
};

class Gate {
    public:
        Gate();
        virtual void update();
        uint32_t input = 0;
        uint32_t output = 0;
        uint32_t inputMask = 0;
        uint32_t outputMask = 0;
        uint8_t inputs = 0;
        uint8_t outputs = 0;
        std::map<uint8_t, Parent*> parents;
        std::map<uint8_t, std::vector<Child*>> children;
        void setInput(uint8_t index, bool value);
        void setOutput(uint8_t index, bool value);
        void setOutputs(uint8_t size);
        void setInputs(uint8_t size);
        bool getInput(uint8_t index) const;
        bool getOutput(uint8_t index) const;
        void recalcInputMask();
        void recalcOutputMask();
        void connect(Gate* gate, uint8_t outputIndex, uint8_t inputIndex);
        void disconnect(Child* child);
};

class AndGate : public Gate {
    public:
        void update() override;
};

class NotGate : public Gate {
    public:
        void update() override;
};

class OnGate : public Gate {
    public:
        OnGate();
};

#endif //TREE_H