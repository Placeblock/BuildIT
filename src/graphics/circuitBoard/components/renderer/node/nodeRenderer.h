//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERENDERER_H
#define BUILDIT_NODERENDERER_H


#include "graphics/programs.h"

#include "graphics/circuitBoard/components/nodes/node.h"
#include "graphics/circuitBoard/components/renderer/componentRenderer.h"

template<typename T>
class NodeRenderer : public Observer<MoveEvent>, public ComponentRenderer, public RenderComponentType<T> {
public:
    void addComponent(T *node) override;
    void removeComponent(T *node) override;

    void addComponent(Component *component) override;
    void removeComponent(Component *component) override;

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

template<typename T>
void NodeRenderer<T>::addComponent(Component *component) {
    RenderComponentType<T>::addComponent(component);
}

template<typename T>
void NodeRenderer<T>::removeComponent(Component *component) {
    RenderComponentType<T>::removeComponent(component);
}

#endif //BUILDIT_NODERENDERER_H
