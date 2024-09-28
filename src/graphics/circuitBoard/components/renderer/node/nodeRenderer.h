//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERENDERER_H
#define BUILDIT_NODERENDERER_H


#include "graphics/programs.h"

#include "graphics/circuitBoard/components/nodes/node.h"
#include "graphics/circuitBoard/components/renderer/renderer.h"

template<typename T>
class NodeRenderer : public Observer<MoveEvent>, public Renderer, public RenderComponentType<T> {
public:
    void addComponent(T *node) override;
    void removeComponent(T *node) override;
    void notify(const MoveEvent &data) override = 0;
};

template<typename T>
void NodeRenderer<T>::addComponent(T *node) {
    node->Movable::subscribe(this);
}

template<typename T>
void NodeRenderer<T>::removeComponent(T *node) {
    node->Movable::unsubscribe(this);
}

#endif //BUILDIT_NODERENDERER_H
