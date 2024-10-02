//
// Created by felix on 9/28/24.
//

#include "componentRenderer.h"
#include "graphics/circuitBoard/components/components.h"

#include <ranges>

void Renderers::addComponent(Component *component) {
    if (this->renderers.contains(typeid(*component))) {
        this->renderers[typeid(*component)]->addComponent(component);
    }
}

void Renderers::removeComponent(Component *component) {
    if (this->renderers.contains(typeid(*component))) {
        this->renderers[typeid(*component)]->removeComponent(component);
    }
}

void Renderers::render(Programs *programs) {
    for (const auto renderer: this->renderers | std::views::values) {
        renderer->render(programs);
    }
}

Renderers::~Renderers() {
    for (const auto &renderer: this->renderers | std::views::values) {
        delete renderer;
    }
}

void ComponentRendererRegistry::registerRenderer(const std::function<RendererData()>& newRendererSupplier) {
    this->rendererSuppliers.push_back(newRendererSupplier);
}

Renderers ComponentRendererRegistry::getNewRenderers() const {
    std::unordered_map<std::type_index, ComponentRenderer*> renderers;
    for (const auto &supplier: this->rendererSuppliers) {
        RendererData data = supplier();
        for (const auto &type: data.types) {
            renderers[type] = data.renderer;
        }
    }
    return Renderers{renderers};
}
