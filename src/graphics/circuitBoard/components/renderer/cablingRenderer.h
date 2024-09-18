//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CABLINGRENDERER_H
#define BUILDIT_CABLINGRENDERER_H


#include "graphics/data/program.h"
#include "graphics/buffer/vertexBuffer.h"
#include "graphics/buffer/vertexArray.h"
#include "graphics/circuitBoard/components/cabling/networkEvents.h"

struct NetworkJoints {
    BufferSection *section;
    std::list<Joint*> joints;
};
struct NetworkWires {
    BufferSection *section;
    std::list<Wire*> wires;
};

class CablingRenderer final : public Observer<MoveEvent>,
                              public Observer<NetworkChangeEvent>,
                              public Observer<NetworkUpdateEvent> {
    VertexArray jointVA;
    SectionedBuffer<VertexData> jointBuffer;
    VertexArray wireVA;
    SectionedBuffer<VertexData> wireBuffer;
    std::unordered_map<Network*, NetworkJoints> jointsSections;
    std::unordered_map<Network*, NetworkWires> wiresSections;
public:
    CablingRenderer();

    void drawWires(const Program* shader);
    void drawJoints(const Program* shader);
    void render(const Program* wireShader, const Program* jointShader);

    void updateJoint(const Joint *joint, glm::vec2 newPos);
    void updateWire(const Wire *wire, glm::vec2 pos, bool start);

    void addJoint(Joint *joint, bool subscribe);
    void removeJoint(Joint *joint, bool subscribe);
    void addWire(Wire *wire, bool subscribe);
    void removeWire(Wire *wire, bool subscribe);

    void updateNetwork(Network *network);

    void notify(const MoveEvent& data) override;
    void notify(const NetworkChangeEvent& data) override;
    void notify(const NetworkUpdateEvent& data) override;
};

#endif //BUILDIT_CABLINGRENDERER_H
