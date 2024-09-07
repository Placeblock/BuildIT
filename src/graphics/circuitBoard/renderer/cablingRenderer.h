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

struct VertexData {
    glm::vec2 pos;
    Color color;
} __attribute__ ((packed));

struct NetworkSection {
    BufferSection *jointsSection;
    BufferSection *wiresSection;
};

class CablingRenderer : public Observer<MoveEvent<Joint>>,
        public Observer<NetworkChangeEvent<Joint>>,
        public Observer<NetworkChangeEvent<Wire>>,
        public Observer<JointAddEvent>, public Observer<JointRemoveEvent>,
        public Observer<WireAddEvent>, public Observer<WireRemoveEvent>,
        public Observer<NetworkAddEvent>, public Observer<NetworkRemoveEvent> {
private:
    VertexArray jointVA;
    SectionedBuffer<VertexData> jointBuffer;
    VertexArray wireVA;
    SectionedBuffer<VertexData> wireBuffer;
    std::unordered_map<Network*, NetworkSection> networkSections;
public:
    CablingRenderer(Subject<JointAddEvent> *jointAddSubject, Subject<JointRemoveEvent> *jointRemoveSubject,
                    Subject<WireAddEvent> *wireAddSubject, Subject<WireRemoveEvent> *wireRemoveSubject,
                    Subject<NetworkAddEvent> *networkAddSubject, Subject<NetworkRemoveEvent> *networkRemoveSubject);

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

    void update(Subject<MoveEvent<Joint>> *subject, const MoveEvent<Joint>& data) override;
    void update(Subject<JointAddEvent> *subject, const JointAddEvent& data) override;
    void update(Subject<JointRemoveEvent> *subject, const JointRemoveEvent& data) override;
    void update(Subject<WireAddEvent> *subject, const WireAddEvent& data) override;
    void update(Subject<WireRemoveEvent> *subject, const WireRemoveEvent& data) override;
    void update(Subject<NetworkAddEvent> *subject, const NetworkAddEvent& data) override;
    void update(Subject<NetworkRemoveEvent> *subject, const NetworkRemoveEvent& data) override;
    void update(Subject<NetworkChangeEvent<Joint>> *subject, const NetworkChangeEvent<Joint>& data) override;
    void update(Subject<NetworkChangeEvent<Wire>> *subject, const NetworkChangeEvent<Wire>& data) override;

};

#endif //BUILDIT_CABLINGRENDERER_H
