#ifndef TREE_H
#define TREE_H

#include <cstdint>
#include <vector>
#include <map>
#include <memory>

class Gate;
class OutputPin;

class Pin {
public:
    Pin(Gate* gate, uint8_t index);
    Gate* gate;
    uint8_t index;
};

class InputPin : public Pin {
public:
    InputPin(Gate* gate, uint8_t index);
    std::shared_ptr<OutputPin> parent = nullptr;
    void set(bool enabled);
    bool get();
};

class OutputPin : public Pin {
public:
    OutputPin(Gate* gate, uint8_t index);
    std::vector<std::shared_ptr<InputPin>> children{};
    void set(bool enabled);
    bool get();
};

class Gate {
    public:
        Gate();
        virtual void update();
        uint32_t input = 0;
        uint32_t output = 0;
        uint32_t inputMask = 0;
        uint32_t outputMask = 0;
        std::vector<std::shared_ptr<InputPin>> inputs;
        std::vector<std::shared_ptr<OutputPin>> outputs;
        void setInput(uint8_t index, bool value);
        void setOutput(uint8_t index, bool value);
        void addOutput(uint8_t index);
        void addInput(uint8_t index);
        void removeOutput(uint8_t index);
        void removeInput(uint8_t index);
        void remove();
        bool getInput(uint8_t index) const;
        bool getOutput(uint8_t index) const;
        void recalcInputMask();
        void recalcOutputMask();
        void connect(Gate* gate, uint8_t outputIndex, uint8_t inputIndex);
        void disconnect(Gate* gate, uint8_t outputIndex, uint8_t inputIndex);
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