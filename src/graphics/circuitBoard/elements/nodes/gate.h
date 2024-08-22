//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_GATE_H
#define BUILDIT_GATE_H

#include <string>
#include <set>
#include <memory>
#include "node.h"
#include "glm/gtx/hash.hpp"
#include "simulation/node.h"
#include "simulation/simulation.h"
#include "graphics/renderer/instancedMeshRenderer.h"
#include "graphics/circuitBoard/renderer/node/instancedNodeRenderer.h"
#include <unordered_set>
#include <unordered_map>

class Gate : public Node {
private:
    InstancedNodeRenderer<Gate>* renderer;
    const std::string text;
    Sim::Simulation* simulation;
    const std::shared_ptr<Sim::Node> simNode;
    static intVec2 calcSize(const std::shared_ptr<Sim::Node>& simNode);
protected:
    std::vector<intVec2> calculateInputPins() override;
    std::vector<intVec2> calculateOutputPins() override;
public:
    Gate(intVec2 pos, InstancedNodeRenderer<Gate>* renderer, std::string text, Sim::Simulation* simulation, std::shared_ptr<Sim::Node> simNode);
    void onMove(intVec2 newPos, bool updateSSBO) override;
    void onInputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputConnect(int index, std::shared_ptr<Vertex> vertex) override;
    void onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) override;
    void addToRenderer() override {
        this->renderer->addNode(this);
    };
    void removeFromRenderer() override {
        this->renderer->removeNode(this);
    };

    ~Gate() override {
        this->renderer->removeInstance(this->pos);
    }
};


#endif //BUILDIT_GATE_H
