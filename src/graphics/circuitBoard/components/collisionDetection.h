//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_COLLISIONDETECTION_H
#define BUILDIT_COLLISIONDETECTION_H

#include <unordered_set>
#include "glm/vec2.hpp"
#include "graphics/circuitBoard/components/abstraction/boundingBox.h"

template <typename T>
class CollisionDetection {
    std::unordered_set<T*> elements;
public:
    T* getColliding(glm::vec2 pos);
    bool isColliding(glm::vec2 pos);
    std::unordered_set<T*> getColliding(BoundingBox boundingBox);
    void addElement(T* element);
    void removeElement(T* element);
};

template<typename T>
std::unordered_set<T *> CollisionDetection<T>::getColliding(BoundingBox boundingBox) {
    std::unordered_set<T*> colliding;
    for (const auto &element: this->elements) {
        if (element->intersects(boundingBox)) {
            colliding.insert(element);
        }
    }
    return colliding;
}

template<typename T>
T *CollisionDetection<T>::getColliding(glm::vec2 pos) {
    for (const auto &element: this->elements) {
        if (element->intersects(pos)) return element;
    }
    return nullptr;
}

template<typename T>
bool CollisionDetection<T>::isColliding(glm::vec2 pos) {
    return this->getColliding(pos) != nullptr;
}

template<typename T>
void CollisionDetection<T>::addElement(T *element) {
    this->elements.insert(element);
}

template<typename T>
void CollisionDetection<T>::removeElement(T *element) {
    this->elements.erase(element);
}


#endif //BUILDIT_COLLISIONDETECTION_H
