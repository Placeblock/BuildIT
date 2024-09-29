//
// Created by felix on 8/3/24.
//

#include "node.h"

#include <utility>
#include "graphics/circuitBoard/serialization/serialize/serializationContext.h"

Node::Node(std::string cnamespace, std::string ckey, glm::vec2 pos, intVec2 cellSize)
    : AABBInteractable(std::move(cnamespace), std::move(ckey), calcBoundingBox(pos, cellSize * 32)),
      Movable(cnamespace, ckey),
      Selectable(cnamespace, ckey),
      Rotatable(cnamespace, ckey),
      pos(pos), cellSize(cellSize) {}

void Node::move(const glm::vec2 newPos) {
    Movable::move(newPos);
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

uint8_t Node::getInputPinIndex(const glm::vec2 absInputPin) {
    const auto pin = uintVec2((absInputPin - glm::vec2(this->getPos())) / 32.0f);
    const auto iter = std::find(this->inputPins.begin(), this->inputPins.end(), pin);
    return std::distance(this->inputPins.begin(), iter);
}

uint8_t Node::getOutputPinIndex(const glm::vec2 absOutputPin) {
    const auto pin = uintVec2((absOutputPin - glm::vec2(this->getPos())) / 32.0f);
    const auto iter = std::find(this->outputPins.begin(), this->outputPins.end(), pin);
    return std::distance(this->outputPins.begin(), iter);
}

void Node::onMove(const glm::vec2 delta) {
    this->pos += delta;
    this->updateBoundingBoxPos(this->pos);
}

glm::vec2 Node::getPos() const {
    return this->pos;
}

Node::Node(const Node &other) : Movable(other), Rotatable(other), Selectable(other),
                                AABBInteractable(other), pos(other.pos), cellSize(other.cellSize) {

}

void Node::serialize(SerializationContext &ctx) {
    intVec2 cell = intVec2(this->getPos() / 32.0f);
    ctx.serialized.write((const char*)&cell.x, sizeof(cell.x));
    ctx.serialized.write((const char*)&cell.y, sizeof(cell.y));
    float rotation = this->getRotation();
    ctx.serialized.write((const char*)&rotation, sizeof(rotation));
    uint32_t input = this->getInput();
    ctx.serialized.write((const char*)&input, sizeof(input));
    uint16_t outputNetworkSize = this->outputNetworks.size();
    ctx.serialized.write((const char*)&outputNetworkSize, sizeof(outputNetworkSize));
    for (const auto &network: this->outputNetworks) {
        uint32_t networkID = ctx.networkIDs.getID(network);
        ctx.serialized.write((const char*)&networkID, sizeof(networkID));
    }
}
