//
// Created by felix on 9/28/24.
//

#ifndef BUILDIT_RENDERER_H
#define BUILDIT_RENDERER_H

#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <functional>
#include <utility>
#include "glm/vec2.hpp"

class Component;
class Programs;

class Renderer {
public:
    virtual void render(Programs *programs) = 0;
    virtual ~Renderer() = default;

    virtual void addComponent(Component *component) = 0;
    virtual void removeComponent(Component *component) = 0;
};

template<typename T>
class RenderComponentType {
public:
    void addComponent(Component *component);
    void removeComponent(Component *component);
    virtual void addComponent(T* component) = 0;
    virtual void removeComponent(T* component) = 0;
};

template<typename T>
void RenderComponentType<T>::addComponent(Component *component) {
    if (auto casted = dynamic_cast<T*>(component)) {
        this->addComponent(casted);
    }
};

template<typename T>
void RenderComponentType<T>::removeComponent(Component *component) {
    if (auto casted = dynamic_cast<T*>(component)) {
        this->removeComponent(casted);
    }
};

class ComponentRenderers {
    std::unordered_map<std::type_index, Renderer*> renderers;

public:
    explicit ComponentRenderers(std::unordered_map<std::type_index, Renderer*> renderers)
        : renderers(std::move(renderers)) {};

    void addComponent(Component *component);
    void removeComponents(Component *component);

    ~ComponentRenderers();
};

struct RendererData {
    std::unordered_set<std::type_index> types;
    Renderer *renderer;
};

class ComponentRendererRegistry {
    std::list<std::function<RendererData()>> rendererSuppliers;

    void registerRenderer(const std::function<RendererData()>& newRendererSupplier);

    ComponentRenderers getNewRenderers();
};


#endif //BUILDIT_RENDERER_H
