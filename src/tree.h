#ifndef TREE_H
#define TREE_H

#include <cstdint>
#include <vector>
#include <map>

struct pin_reference {
    struct Gate* gate;
    int index;
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
        std::vector<std::vector<pin_reference*>> children;
        void setInput(uint8_t index, bool value);
        void setOutput(uint8_t index, bool value);
        void setOutputs(uint8_t size);
        void setInputs(uint8_t size);
        bool getInput(uint8_t index) const;
        bool getOutput(uint8_t index) const;
        void recalcInputMask();
        void recalcOutputMask();
        void connect(size_t index, pin_reference* child);
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