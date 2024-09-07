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

class CablingRenderer : public MultiObserver<MoveEvent<Joint>, Joint*>,
        public Observer<JointAddEvent>, public Observer<JointRemoveEvent>,
        public Observer<WireAddEvent>, public Observer<WireRemoveEvent>,
        public Observer<NetworkAddEvent>, public Observer<NetworkRemoveEvent> {
private:
    VertexArray jointVA;
    SectionedBuffer<VertexData> jointBuffer;
    VertexArray wireVA;
    SectionedBuffer<VertexData> wireBuffer;
    std::unordered_map<Network*, NetworkSection> networkSections;

    Subject<JointAddEvent> *jointAddSubject;
    Subject<JointRemoveEvent> *jointRemoveSubject;
    Subject<WireAddEvent> *wireAddSubject;
    Subject<WireRemoveEvent> *wireRemoveSubject;
    Subject<NetworkAddEvent> *networkAddSubject;
    Subject<NetworkRemoveEvent> *networkRemoveSubject;
public:
    CablingRenderer(Subject<JointAddEvent> *jointAddSubject, Subject<JointRemoveEvent> *jointRemoveSubject,
                    Subject<WireAddEvent> *wireAddSubject, Subject<WireRemoveEvent> *wireRemoveSubject,
                    Subject<NetworkAddEvent> *networkAddSubject, Subject<NetworkRemoveEvent> *networkRemoveSubject);

    void drawWires(Program* shader);
    void drawJoints(Program* shader);
    void render(Program* wireShader, Program* jointShader);

    void updateJoint(Joint *joint);
    void updateWire(Wire *wire);

    void updateNetwork(Network *network);

    void update(const MoveEvent<Joint>& data, Joint *joint) override;
    void update(const JointAddEvent& data) override;
    void update(const JointRemoveEvent& data) override;
    void update(const WireAddEvent& data) override;
    void update(const WireRemoveEvent& data) override;
    void update(const NetworkAddEvent& data) override;
    void update(const NetworkRemoveEvent& data) override;

    ~CablingRenderer() override;
};

#endif //BUILDIT_CABLINGRENDERER_H
