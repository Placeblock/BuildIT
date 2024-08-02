//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include "cables.h"
#include "util.h"

void Network::deleteCable(std::shared_ptr<Cable> cable) {
    removeByValue(&cable->start->cables, cable);
    removeByValue(&cable->end->cables, cable);
    removeByValue(&this->cables, cable);
}

void Network::deleteVertex(std::shared_ptr<Vertex> vertex) {
    removeByValue(&this->vertices, vertex);
}

std::shared_ptr<Vertex> Cables::getVertex(glm::vec2 cell) const {
    if (const auto result = this->cellMap.find(cell); result != this->cellMap.end()) {
        return result->second;
    }
    return nullptr;
}

std::shared_ptr<Cable> Cables::getCable(glm::vec2 cell) {
    const auto iter = std::find_if(this->cableMap.begin(), this->cableMap.end(),
       [&cell](const std::pair<std::shared_ptr<Cable>, std::shared_ptr<Network>> cable) {
           const auto left = cable.first->start->cell - cell;
           const auto right = cable.first->end->cell - cell;
           return left.x*right.y - left.y*right.x == 0 &&
                  left.x*right.x + left.y*right.y < 0;
       });
    if (iter != this->cableMap.end()) return iter->first;
    return nullptr;
}

std::shared_ptr<Network> Cables::getNetwork(std::shared_ptr<Vertex> vertex) {
    if (const auto result = this->vertexMap.find(vertex); result != this->vertexMap.end()) {
        return result->second;
    }
    return nullptr;
}

void Cables::deleteVertex(std::shared_ptr<Vertex> vertex) {
    this->vertexMap.erase(vertex);
    this->cellMap.erase(vertex->cell);
}

void Cables::deleteCable(std::shared_ptr<Cable> cable) {
    this->cableMap.erase(cable);
}

std::shared_ptr<Vertex> Cable::getOther(std::shared_ptr<Vertex> cell) {
    if (cell == this->start) return this->end;
    return this->start;
}
