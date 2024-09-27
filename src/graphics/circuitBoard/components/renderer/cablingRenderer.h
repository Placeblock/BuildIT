//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CABLINGRENDERER_H
#define BUILDIT_CABLINGRENDERER_H


#include "graphics/data/program.h"
#include "graphics/buffer/vertexBuffer.h"
#include "graphics/buffer/vertexArray.h"
#include "graphics/uniqueBlockingQueue.h"
#include "graphics/threadPool.h"
#include "graphics/circuitBoard/events/networkChangeEvent.h"
#include "graphics/circuitBoard/events/networkEvents.h"
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/events/componentSelectEvent.h"

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
                              public Observer<NetworkUpdateEvent> {
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

    void drawWires(const Program* shader);
    void drawJoints(const Program* shader);
    void render(const Program* wireShader, const Program* jointShader);

    void moveJoint(Joint *joint, glm::vec2 delta);

    void addJoint(Joint *joint, bool subscribe);
    void removeJoint(Joint *joint, bool subscribe);
    void addWire(Wire *wire, bool subscribe);
    void removeWire(Wire *wire, bool subscribe);

    void updateNetwork(Network *network);

    bool hasNetwork(Network *network);
    void checkNetworkUnsubscribe(Network *network);

    void notify(const MoveEvent& data) override;
    void notify(const SelectEvent& data) override;
    void notify(const DeselectEvent& data) override;
    void notify(const NetworkChangeEvent& data) override;
    void notify(const NetworkUpdateEvent& data) override;
};

#endif //BUILDIT_CABLINGRENDERER_H
