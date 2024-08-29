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
#include "graphics/circuitBoard/components/nodes/renderer/instancedNodeRenderer.h"
#include <unordered_set>
#include <unordered_map>

class Gate : public Node {
private:
    const std::string text;
    static intVec2 calcSize(const std::shared_ptr<Sim::Node>& simNode);
protected:
    std::vector<uintVec2> calculateInputPins() override;
    std::vector<uintVec2> calculateOutputPins() override;
public:
    Gate(glm::vec2 cell, InstancedNodeRenderer* renderer, std::string text, const std::shared_ptr<Sim::Node>& simNode);
    void onMove(glm::vec2 newCell, bool updateBuffer) override;

    ~Gate() override {
        static_cast<InstancedNodeRenderer*>(this->renderer)->removeInstance(this->cell);
    }
};


#endif //BUILDIT_GATE_H
