//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CABLES_H
#define BUILDIT_CABLES_H

#include <vector>
#include <map>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

class Cable;
class Network;

class Vertex {
public:
    glm::vec2 cell;
    glm::vec3 color;
    std::vector<Cable*> cables;
    Network* network;
};

class Cable {
public:
    Vertex* start;
    Vertex* end;
    glm::vec3 color;
    Network* network;
};

class Network {
public:
    std::vector<Cable*> cables;
    std::vector<Vertex*> vertices;
    void deleteCable(Cable* cable); // vertices are only deleted if they have no more cables
    void deleteVertex(Vertex* vertex);
};

class Cables {
public:
    std::vector<Network*> networks;
    std::unordered_map<glm::vec2, Vertex*> cellMap;
    std::unordered_map<Vertex*, Network*> vertexMap;
    std::unordered_map<Cable*, Network*> cableMap;
    [[nodiscard]] Vertex* getVertex(glm::vec2 cell) const;
    Cable* getCable(glm::vec2 cable);
    Network* getNetwork(Vertex* vertex);
    void deleteVertex(Vertex* vertex);
    void deleteCable(Cable* cable);
};


#endif //BUILDIT_CABLES_H
