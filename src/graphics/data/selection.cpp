//
// Created by felix on 8/9/24.
//

#include "selection.h"

void Selection::clear() {
    for (const auto &item: this->vertices) {
        this->removeVertex(item, false);
    }
    this->vertices.clear();
}

void Selection::addVertex(const Vertex* vertex) {
    this->vertices.insert(vertex);
    const long index = this->wires->getVertexIndex(vertex);
    this->renderer->updateVertexColor(int(index), selectedVertexColor);
}

void Selection::removeVertex(const Vertex* vertex, bool erase) {
    if (erase) {
        this->vertices.erase(vertex);
    }
    const long index = this->wires->getVertexIndex(vertex);
    this->renderer->updateVertexColor(int(index), vertex->color);
}
