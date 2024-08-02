//
// Created by felix on 8/2/24.
//

#include "createVertexAction.h"

void CreateVertexAction::Execute(Cables* cables) {
    cables->cellMap[this->vertex->cell] = this->vertex;
    if (!this->createdNetwork) { // Don't create new network if action is used as redo
        this->createdNetwork = std::make_shared<Network>();
    }
    this->createdNetwork->vertices.push_back(this->vertex);
    cables->networks.push_back(this->createdNetwork);
}

void CreateVertexAction::Rewind(Cables* cables) {
    this->createdNetwork->vertices.pop_back();
    cables->networks.pop_back();
    cables->cellMap.erase(this->vertex->cell);
}

CreateVertexAction::CreateVertexAction(std::shared_ptr<Vertex> vertex) {
    this->vertex = vertex;
}
