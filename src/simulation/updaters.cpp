//
// Created by felix on 16.07.24.
//

#include "updaters.h"

using namespace Sim;

uint32_t NotUpdater::update(uint32_t input, uint32_t inputMask, uint32_t outputMask) {
    return ~input;
}

std::unique_ptr<Updater> NotUpdater::clone() {
    return std::make_unique<NotUpdater>(*this);
}

uint32_t AndUpdater::update(uint32_t input, uint32_t inputMask, uint32_t outputMask) {
    return (input & inputMask) == inputMask ? outputMask : 0;
}

std::unique_ptr<Updater> AndUpdater::clone() {
    return std::make_unique<AndUpdater>(*this);
}

TruthTableUpdater::TruthTableUpdater(std::unordered_map<uint32_t, uint32_t> &truthTable) : truthTable(truthTable) {

}

uint32_t TruthTableUpdater::update(uint32_t input, uint32_t inputMask, uint32_t outputMask) {
    return this->truthTable[input];
}

std::unique_ptr<Updater> TruthTableUpdater::clone() {
    return std::make_unique<TruthTableUpdater>(this->truthTable);
}
