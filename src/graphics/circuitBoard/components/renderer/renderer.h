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

class Component;
class Programs;

class Renderer {
public:
    virtual void render(Programs *programs) = 0;
    virtual ~Renderer() = default;
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
    if (auto casted = static_cast<T*>(component)) {
        this->addComponent(casted);
    }
};

template<typename T>
void RenderComponentType<T>::removeComponent(Component *component) {
    if (auto casted = static_cast<T*>(component)) {
        this->removeComponent(casted);
    }
};

class Renderers {
    std::unordered_map<std::type_index, Renderer*> renderers;

public:
    explicit Renderers(std::unordered_map<std::type_index, Renderer*> renderers)
        : renderers(std::move(renderers)) {};

    void addComponent(Component *component);
    void removeComponent(Component *component);

    void render(Programs *programs);

    ~Renderers();
};

struct RendererData {
    std::unordered_set<std::type_index> types;
    Renderer *renderer;
};

class ComponentRendererRegistry {
    std::list<std::function<RendererData()>> rendererSuppliers;
public:
    void registerRenderer(const std::function<RendererData()>& newRendererSupplier);

    [[nodiscard]] Renderers getNewRenderers() const;
};


#endif //BUILDIT_RENDERER_H
