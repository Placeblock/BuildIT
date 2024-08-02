//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTVERTEXACTION_H
#define BUILDIT_INSERTVERTEXACTION_H


#include "cables/action.h"

class InsertVertexAction : public Action {
private:
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Cable> splitCable;
    std::shared_ptr<Cable> createdCables[2];
public:
    explicit InsertVertexAction(std::shared_ptr<Vertex> vertex);
    void Execute(Cables* cables) override;
    void Rewind(Cables* cables) override;
};


#endif //BUILDIT_INSERTVERTEXACTION_H
