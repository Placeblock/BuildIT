//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CABLINGRENDERER_H
#define BUILDIT_CABLINGRENDERER_H


#include "graphics/data/program.h"
#include "graphics/buffer/vertexBuffer.h"
#include "graphics/buffer/vertexArray.h"
#include "graphics/circuitBoard/events/networkEvents.h"
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/events/componentSelectEvent.h"
#include "componentRenderer.h"
#include "graphics/circuitBoard/components/cabling/wire.h"

struct WireIndices {
    Index* startVertexIndex;
    Index* startColorIndex;
    Index* endVertexIndex;
    Index* endColorIndex;
};
struct JointIndices {
    Index* vertexIndex;
    Index* colorIndex;
};

class CablingRenderer final : public Observer<MoveEvent>,
                              public Observer<SelectEvent>,
                              public Observer<DeselectEvent>,
                              public Observer<NetworkChangeEvent>,
                              public Observer<NetworkUpdateEvent>,
                              public RenderComponentType<Wire>,
                              public RenderComponentType<Joint>,
                              public ComponentRenderer {
    VertexArray jointVA;
    IndexedBuffer<glm::vec2> jointVertexBuffer;
    IndexedBuffer<Color> jointColorBuffer;
    VertexArray wireVA;
    IndexedBuffer<glm::vec2> wireVertexBuffer;
    IndexedBuffer<Color> wireColorBuffer;
    std::unordered_map<Joint*, glm::vec2> jointPositions;
    std::unordered_map<Joint*, JointIndices> jointsIndices;
    std::unordered_map<Wire*, WireIndices> wiresIndices;

    void updateJoint(Joint *joint, glm::vec2 newPos);
    void updateJoint(Joint *joint, Color newColor);
    void updateWire(Wire *wire, glm::vec2 newPos, bool start);
    void updateWire(Wire *wire, Color newColor);
public:
    CablingRenderer();

    void renderWires(const Program* shader);
    void renderJoints(const Program* shader);

    void moveJoint(Joint *joint, glm::vec2 delta);

    void updateNetwork(Network *network);

    bool hasNetwork(Network *network);
    void checkNetworkUnsubscribe(Network *network);

    void notify(const MoveEvent& data) override;
    void notify(const SelectEvent& data) override;
    void notify(const DeselectEvent& data) override;
    void notify(const NetworkChangeEvent& data) override;
    void notify(const NetworkUpdateEvent& data) override;

    void render(Programs *programs) override;

    void addComponent(Component *component) override;
    void removeComponent(Component *component) override;

    void addComponent(Wire* component) override;
    void addComponent(Joint* component) override;
    void removeComponent(Wire* component) override;
    void removeComponent(Joint* component) override;

    void addWire(Wire* wire, bool subscribe);
    void addJoint(Joint* joint, bool subscribe);
    void removeWire(Wire* wire, bool unsubscribe);
    void removeJoint(Joint* joint, bool unsubscribe);
};

#endif //BUILDIT_CABLINGRENDERER_H
