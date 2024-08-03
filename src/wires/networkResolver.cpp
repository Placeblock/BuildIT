//
// Created by felix on 8/3/24.
//

#include "networkResolver.h"

#include <utility>

NetworkResolver::NetworkResolver(std::set<std::shared_ptr<Vertex>> vertices) {
    this->vertices = std::move(vertices);
}

void NetworkResolver::resolve() {
    std::set<std::shared_ptr<Vertex>> processingQueue;
    for (const auto &vertex: this->vertices) {
        processingQueue.insert(vertex);
    }
    while (!processingQueue.empty()) {
        const auto vertex = *processingQueue.begin();
        processingQueue.erase(vertex);
        std::set<std::shared_ptr<Vertex>> networkVertices;
        std::queue<std::shared_ptr<Vertex>> networkQueue;

        networkQueue.push(vertex);
        while (!networkQueue.empty()) {
            const auto networkVertex = networkQueue.front();
            networkQueue.pop();
            processingQueue.erase(networkVertex);
            networkVertices.insert(networkVertex);
            for (const auto &vertexWires: networkVertex->wires) {
                const auto otherVertex = vertexWires->getOther(networkVertex);
                if (networkVertices.contains(otherVertex)) continue;
                networkQueue.push(otherVertex);
            }
        }
        this->resolved.push_back(networkVertices);
    }
}
