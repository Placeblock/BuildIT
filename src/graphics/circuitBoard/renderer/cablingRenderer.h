//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CABLINGRENDERER_H
#define BUILDIT_CABLINGRENDERER_H


#include <GL/glew.h>
#include <vector>
#include <map>
#include "graphics/data/program.h"
#include "graphics/circuitBoard/components/cabling/jointContainer.h"
#include "graphics/circuitBoard/components/cabling/wireContainer.h"
#include "graphics/buffer/vertexBuffer.h"
#include "graphics/buffer/vertexArray.h"
#include "graphics/circuitBoard/components/cabling/networkEvents.h"

struct VertexData {
    glm::vec2 pos;
    Color color;
} __attribute__ ((packed));

struct NetworkSection {
    BufferSection *jointsSection;
    BufferSection *wiresSection;
};

class CablingRenderer : public CastedObserver<MoveEvent, Joint>,
                        public CastedObserver<NetworkChangeEvent, Joint>,
                        public CastedObserver<NetworkChangeEvent, Wire>,
                        public Observer<ComponentAddEvent>,
                        public Observer<ComponentRemoveEvent>,
                        public Observer<WireAddEvent>, public Observer<WireRemoveEvent>,
                        public Observer<NetworkAddEvent>, public Observer<NetworkRemoveEvent> {
private:
    VertexArray jointVA;
    SectionedBuffer<VertexData> jointBuffer;
    VertexArray wireVA;
    SectionedBuffer<VertexData> wireBuffer;
    std::unordered_map<Network*, NetworkSection> networkSections;
public:
    CablingRenderer();

    void drawWires(Program* shader);
    void drawJoints(Program* shader);
    void render(Program* wireShader, Program* jointShader);

    void addJoint(Joint *joint);
    void removeJoint(Joint *joint);
    void addWire(Wire *wire);
    void removeWire(Wire *wire);

    void updateJoint(Joint *joint);
    void updateWire(Wire *wire);

    void updateNetwork(Network *network);

    void notify(Joint *joint, const MoveEvent& data) override;
    void notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) override;
    void notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) override;
    void notify(Subject<WireAddEvent> *subject, const WireAddEvent& data) override;
    void notify(Subject<WireRemoveEvent> *subject, const WireRemoveEvent& data) override;
    void notify(Subject<NetworkAddEvent> *subject, const NetworkAddEvent& data) override;
    void notify(Subject<NetworkRemoveEvent> *subject, const NetworkRemoveEvent& data) override;
    void notify(Joint *joint, const NetworkChangeEvent& data) override;
    void notify(Wire *wire, const NetworkChangeEvent& data) override;

};

#endif //BUILDIT_CABLINGRENDERER_H
