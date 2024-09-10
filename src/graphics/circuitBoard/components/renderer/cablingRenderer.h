//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CABLINGRENDERER_H
#define BUILDIT_CABLINGRENDERER_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "graphics/data/program.h"
#include "graphics/circuitBoard/components/cabling/wireContainer.h"
#include "graphics/buffer/vertexBuffer.h"
#include "graphics/buffer/vertexArray.h"
#include "graphics/circuitBoard/components/cabling/networkEvents.h"

struct NetworkSection {
    BufferSection *jointsSection;
    BufferSection *wiresSection;
};

class CablingRenderer : public Observer<MoveEvent>, public Observer<NetworkChangeEvent> {
private:
    VertexArray jointVA;
    SectionedBuffer<VertexData> jointBuffer;
    VertexArray wireVA;
    SectionedBuffer<VertexData> wireBuffer;
    std::unordered_map<Network*, NetworkSection> networkSections;

    void addJoint(Joint *joint);
    void removeJoint(Joint *joint);
    void addWire(Wire *wire);
    void removeWire(Wire *wire);
public:
    CablingRenderer();

    void drawWires(Program* shader);
    void drawJoints(Program* shader);
    void render(Program* wireShader, Program* jointShader);

    void updateJoint(Joint *joint, glm::vec2 newPos);
    void updateWire(Wire *wire, glm::vec2 pos, bool start);

    void addNetwork(Network *network);
    void removeNetwork(Network *network);

    void updateNetwork(Network *network);

    void notify(const MoveEvent& data) override;
    void notify(const NetworkChangeEvent& data) override;
};

#endif //BUILDIT_CABLINGRENDERER_H
