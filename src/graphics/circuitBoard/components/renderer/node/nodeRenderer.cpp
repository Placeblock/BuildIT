//
// Created by felix on 9/7/24.
//

#include "nodeRenderer.h"

template<typename T>
void NodeRenderer<T>::addNode(T *node) {
    node->Movable::subscribe(this);
}

template<typename T>
void NodeRenderer<T>::removeNode(T *node) {
    node->Movable::unsubscribe(this);
}
