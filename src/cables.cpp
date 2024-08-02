//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include <stdexcept>
#include "cables.h"
#include "util.h"

void Network::deleteCable(Cable *cable) {
    removeByValue(&cable->start->cables, cable);
    removeByValue(&cable->end->cables, cable);
    removeByValue(&this->cables, cable);
    delete cable;
}

void Network::deleteVertex(Vertex *vertex) {
    if (!vertex->cables.empty()) {
        throw std::logic_error("Tried to delete Vertex with connected lines");
    }
    removeByValue(&this->vertices, vertex);
    this->vertices.erase(std::remove(this->vertices.begin(), this->vertices.end(), vertex), this->vertices.end());
    delete vertex;
}

Vertex *Cables::getVertex(glm::vec2 cell) const {
    if (const auto result = this->cellMap.find(cell); result != this->cellMap.end()) {
        return result->second;
    }
    return nullptr;
}

Cable *Cables::getCable(glm::vec2 cell) {
    const auto iter = std::find_if(this->cableMap.begin(), this->cableMap.end(),
       [&cell](const std::pair<Cable*, Network*> cable) {
           const auto left = cable.first->start->cell - cell;
           const auto right = cable.first->end->cell - cell;
           return left.x*right.y - left.y*right.x == 0 &&
                  left.x*right.x + left.y*right.y < 0;
       });
    if (iter != this->cableMap.end()) return iter->first;
    return nullptr;
}

Network *Cables::getNetwork(Vertex *vertex) {
    if (const auto result = this->vertexMap.find(vertex); result != this->vertexMap.end()) {
        return result->second;
    }
    return nullptr;
}

void Cables::deleteVertex(Vertex *vertex) {
    this->vertexMap.erase(vertex);
    this->cellMap.erase(vertex->cell);
}

void Cables::deleteCable(Cable *cable) {
    this->cableMap.erase(cable);
}
