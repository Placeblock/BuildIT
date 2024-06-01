#ifndef TREE_H
#define TREE_H

#include <cstdint>
#include <vector>
#include <map>

struct Connection {
    struct Gate* parent;
    struct Gate* child;
    uint8_t output;
    uint8_t input;
};

class ConnectionManager {
public:
    std::map<Gate*, std::vector<Connection*>> children;
    std::map<Gate*, std::vector<Connection*>> parents;
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
        void setInput(uint8_t index, bool value);
        void setOutput(uint8_t index, bool value);
        void setOutputs(uint8_t size);
        void setInputs(uint8_t size);
        bool getInput(uint8_t index) const;
        bool getOutput(uint8_t index) const;
        void recalcInputMask();
        void recalcOutputMask();
        Connection* connect(ConnectionManager* manager, Gate* gate, uint8_t outputIndex, uint8_t inputIndex);
        void disconnect(ConnectionManager* manager, Connection* connection);
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