//
// Created by felix on 8/9/24.
//

#include "selection.h"

void Selection::clear() {
    for (const auto &item: this->vertices) {
        this->removeVertex(item);
    }
}

void Selection::addVertex(const std::shared_ptr<Vertex>& vertex) {
    this->vertices.insert(vertex);
    const long index = this->wires->getVertexIndex(vertex);
    this->renderer->updateVertexColor(int(index), selectedVertexColor);
}

void Selection::removeVertex(const std::shared_ptr<Vertex>& vertex) {
    this->vertices.insert(vertex);
    const long index = this->wires->getVertexIndex(vertex);
    this->renderer->updateVertexColor(int(index), vertex->color);
}
