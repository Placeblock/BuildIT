//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CABLES_H
#define BUILDIT_CABLES_H

#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include "glm/glm.hpp"

class Cable;
class Network;

class Vertex {
public:
    glm::vec2 cell;
    glm::vec3 color;
    std::vector<std::shared_ptr<Cable>> cables;
    std::shared_ptr<Network> network;
};

class Cable {
public:
    std::shared_ptr<Vertex> start;
    std::shared_ptr<Vertex> end;
    std::shared_ptr<Network> network;
    std::shared_ptr<Vertex> getOther(std::shared_ptr<Vertex> cell);
};

class Network {
public:
    glm::vec3 color;
    std::vector<std::shared_ptr<Cable>> cables;
    std::vector<std::shared_ptr<Vertex>> vertices;
    void deleteCable(std::shared_ptr<Cable> cable); // vertices are only deleted if they have no more cables
    void deleteVertex(std::shared_ptr<Vertex> vertex);
};

class Cables {
public:
    std::vector<std::shared_ptr<Network>> networks;
    std::unordered_map<glm::vec2, std::shared_ptr<Vertex>> cellMap;
    std::unordered_map<std::shared_ptr<Vertex>, std::shared_ptr<Network>> vertexMap;
    std::unordered_map<std::shared_ptr<Cable>, std::shared_ptr<Network>> cableMap;
    [[nodiscard]] std::shared_ptr<Vertex> getVertex(glm::vec2 cell) const;
    std::shared_ptr<Cable> getCable(glm::vec2 cable);
    std::shared_ptr<Network> getNetwork(std::shared_ptr<Vertex> vertex);
    void deleteVertex(std::shared_ptr<Vertex> vertex);
    void deleteCable(std::shared_ptr<Cable> cable);
};


#endif //BUILDIT_CABLES_H
