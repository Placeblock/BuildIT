//
// Created by felix on 9/9/24.
//

#include "createComponentAction.h"

CreateComponentAction::CreateComponentAction(ComponentContainer *compContainer,
                                             const std::shared_ptr<Component> &component, bool reversed) :
                                             compContainer(compContainer), component(component),
                                             Action(reversed) {

}

void CreateComponentAction::execute(bool lastInBatch) {
    this->compContainer->addComponent(this->component);
}

void CreateComponentAction::rewind(bool lastInBatch) {
    this->compContainer->removeComponent(this->component.get());
}
