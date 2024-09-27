//
// Created by felix on 9/27/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include "saveFeature.h"
#include "graphics/circuitBoard/components/componentContainer.h"
#include "graphics/circuitBoard/serialization/serialize/binarySerializationVisitor.h"

void SaveFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (key != GLFW_KEY_S || mods != GLFW_MOD_CONTROL) return;
    std::unique_lock<std::mutex> lock{this->simulation->modifyLock};
    BinarySerializationVisitor visitor{this->componentContainer->getComponents(), this->simulation->getUpdateQueue()};
    visitor.serialize();
    std::ofstream fs;
    fs.open("save.buildit", std::fstream::out | std::fstream::binary | std::fstream::trunc);
    fs << visitor.serialized.rdbuf();
    fs.close();
}

SaveFeature::SaveFeature(ComponentContainer *componentContainer, Sim::Simulation *simulation)
    : componentContainer(componentContainer), simulation(simulation) {

}
