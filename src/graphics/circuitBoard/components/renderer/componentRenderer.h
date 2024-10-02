//
// Created by felix on 9/28/24.
//

#ifndef BUILDIT_COMPONENTRENDERER_H
#define BUILDIT_COMPONENTRENDERER_H

#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <functional>
#include <utility>

class Component;
class Programs;

class ComponentRenderer {
public:
    virtual void addComponent(Component *component) = 0;
    virtual void removeComponent(Component *component) = 0;

    virtual void render(Programs *programs) = 0;
    virtual ~ComponentRenderer() = default;
};

template<typename T>
class RenderComponentType {
public:
    virtual void addComponent(Component *component);
    virtual void removeComponent(Component *component);
    virtual void addComponent(T* component) = 0;
    virtual void removeComponent(T* component) = 0;

    virtual ~RenderComponentType() = default;
};

template<typename T>
void RenderComponentType<T>::addComponent(Component *component) {
    if (auto casted = dynamic_cast<T*>(component)) {
        this->addComponent(casted);
    }
}

template<typename T>
void RenderComponentType<T>::removeComponent(Component *component) {
    if (auto casted = dynamic_cast<T*>(component)) {
        this->removeComponent(casted);
    }
}

class Renderers {
    std::unordered_map<std::type_index, ComponentRenderer*> renderers;

public:
    explicit Renderers(std::unordered_map<std::type_index, ComponentRenderer*> renderers)
        : renderers(std::move(renderers)) {};

    void addComponent(Component *component);
    void removeComponent(Component *component);

    void render(Programs *programs);

    ~Renderers();
};

struct RendererData {
    std::unordered_set<std::type_index> types;
    ComponentRenderer *renderer;
};

class ComponentRendererRegistry {
    std::list<std::function<RendererData()>> rendererSuppliers;
public:
    void registerRenderer(const std::function<RendererData()>& newRendererSupplier);

    [[nodiscard]] Renderers getNewRenderers() const;
};


#endif //BUILDIT_COMPONENTRENDERER_H
