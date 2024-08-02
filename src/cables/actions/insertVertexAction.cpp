//
// Created by felix on 8/3/24.
//

#include "insertVertexAction.h"

InsertVertexAction::InsertVertexAction(std::shared_ptr<Vertex> vertex) {
    this->vertex = vertex;
}

void InsertVertexAction::Execute(Cables *cables) {
    if (!this->createdCables[0] || !this->createdCables[1]) {
        this->splitCable = cables->getCable(this->vertex->cell);
        this->createdCables[0]->start = this->splitCable->start;
        this->createdCables[0]->end = this->vertex;
        this->createdCables[0]->network = this->splitCable->network;
        this->createdCables[1]->start = this->vertex;
        this->createdCables[1]->end = this->splitCable->end;
        this->createdCables[1]->network = this->splitCable->network;
    }

    cables->deleteCable(this->splitCable);
    this->splitCable->network->deleteCable(this->splitCable);

    cables->cellMap[this->vertex->cell] = this->vertex;
    this->splitCable->network->vertices.push_back(this->vertex);

    cables->cableMap[this->createdCables[0]] = this->splitCable->network;
    cables->cableMap[this->createdCables[1]] = this->splitCable->network;
    this->splitCable->network->cables.push_back(this->createdCables[0]);
    this->splitCable->network->cables.push_back(this->createdCables[1]);

    this->splitCable->start->cables.push_back(this->createdCables[0]);
    this->vertex->cables.push_back(this->createdCables[0]);
    this->vertex->cables.push_back(this->createdCables[1]);
    this->splitCable->end->cables.push_back(this->createdCables[1]);
}

void InsertVertexAction::Rewind(Cables *cables) {
    if (!this->splitCable) {
        this->splitCable = std::make_shared<Cable>();
        this->splitCable->start = this->vertex->cables[0]->getOther(this->vertex);
        this->splitCable->end = this->vertex->cables[1]->getOther(this->vertex);
        this->splitCable->network = this->vertex->network;
    }

    this->vertex->network->deleteCable(this->vertex->cables[0]);
    this->vertex->network->deleteCable(this->vertex->cables[1]);
    cables->deleteCable(this->vertex->cables[0]);
    cables->deleteCable(this->vertex->cables[1]);

    this->vertex->network->deleteVertex(this->vertex);
    cables->deleteVertex(this->vertex);

    this->splitCable->start->cables.push_back(this->splitCable);
    this->splitCable->end->cables.push_back(this->splitCable);
    this->vertex->network->cables.push_back(this->splitCable);
    cables->cableMap[this->splitCable] = this->vertex->network;
}
