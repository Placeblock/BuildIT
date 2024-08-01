//
// Created by felix on 7/31/24.
//

#include "gateNode.h"
#include "gate.h"

template<typename N>
Graphics::GateNode<N>::GateNode(Vector2 pos, N *simNode) : Node(pos) {
    this->simNode = simNode;
}

/*
 * We have to tell the compiler which template type we use, so it generates
 * the corresponding constructors of GateNode!
 */
template class Graphics::GateNode<Sim::NotGate>;
template class Graphics::GateNode<Sim::AndGate>;
