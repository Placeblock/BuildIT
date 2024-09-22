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
    BufferSection *vertexSection;
    BufferSection *colorSection;
    std::list<Joint*> joints;
};
struct NetworkWires {
    BufferSection *vertexSection;
    BufferSection *colorSection;
    std::list<Wire*> wires;
};

struct ElementData {
    BufferSection *vertexSection;
    BufferSection *colorSection;
    int index;
};

class CablingRenderer final : public Observer<MoveEvent>,
                              public Observer<SelectEvent>,
                              public Observer<DeselectEvent>,
                              public Observer<NetworkChangeEvent>,
                              public Observer<NetworkUpdateEvent> {
    VertexArray jointVA;
    SectionedBuffer<glm::vec2> jointVertexBuffer;
    SectionedBuffer<Color> jointColorBuffer;
    VertexArray wireVA;
    SectionedBuffer<glm::vec2> wireVertexBuffer;
    SectionedBuffer<Color> wireColorBuffer;
    std::unordered_map<Network*, NetworkJoints> jointsSections;
    std::unordered_map<Network*, NetworkWires> wiresSections;
    std::unordered_map<Joint*, glm::vec2> jointPositions;

    glm::vec2 getJointPos(Joint *joint);

    ElementData getJointData(const Joint *joint);
    ElementData getWireData(const Wire *wire);
    void updateJoint(const Joint *joint, glm::vec2 newPos);
    void updateJoint(const Joint *joint, Color newColor);
    void updateWire(const Wire *wire, glm::vec2 newPos, bool start);
    void updateWire(const Wire *wire, Color newColor);
public:
    CablingRenderer();

    void drawWires(const Program* shader);
    void drawJoints(const Program* shader);
    void render(const Program* wireShader, const Program* jointShader);

    void moveJoint(Joint *joint, glm::vec2 delta);
    void moveWire(Wire *wire, glm::vec2 delta);

    void addJoint(Joint *joint, bool subscribe);
    void removeJoint(Joint *joint, bool subscribe);
    void addWire(Wire *wire, bool subscribe);
    void removeWire(Wire *wire, bool subscribe);

    void updateNetwork(Network *network);

    void notify(const MoveEvent& data) override;
    void notify(const SelectEvent& data) override;
    void notify(const DeselectEvent& data) override;
    void notify(const NetworkChangeEvent& data) override;
    void notify(const NetworkUpdateEvent& data) override;
};

#endif //BUILDIT_CABLINGRENDERER_H
