//
// Created by felix on 9/28/24.
//

#include "renderer.h"

void ComponentRenderers::addComponent(Component *component) {
    if (this->renderers.contains(typeid(component))) {
        this->renderers[typeid(component)]->addComponent(component);
    }
}

void ComponentRenderers::removeComponents(Component *component) {
    if (this->renderers.contains(typeid(component))) {
        this->renderers[typeid(component)]->removeComponent(component);
    }
}

ComponentRenderers::~ComponentRenderers() {
    for (const auto &[type, renderer]: this->renderers) {
        delete renderer;
    }
}

void ComponentRendererRegistry::registerRenderer(const std::function<RendererData()>& newRendererSupplier) {
    this->rendererSuppliers.push_back(newRendererSupplier);
}

ComponentRenderers ComponentRendererRegistry::getNewRenderers() {
    std::unordered_map<std::type_index, Renderer*> renderers;
    for (const auto &supplier: this->rendererSuppliers) {
        RendererData data = supplier();
        for (const auto &type: data.types) {
            renderers[type] = data.renderer;
        }
    }
    return ComponentRenderers{renderers};
}
