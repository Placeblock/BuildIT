//
// Created by felix on 29.03.25.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Node.h"

namespace Sim {
    template<typename T>
    struct weak_ptr_hash {
        size_t operator()(const std::weak_ptr<T> &wp) const {
            // Compute the hash based on the underlying shared_ptr
            auto sp = wp.lock(); // Get the shared_ptr from the weak_ptr
            return sp ? std::hash<T *>()(sp.get()) : 0;
        }
    };

    // Custom comparison function for weak_ptr
    template<typename T>
    struct weak_ptr_equal {
        bool operator()(const std::weak_ptr<T> &wp1, const std::weak_ptr<T> &wp2) const {
            // Compare weak_ptrs by checking if the locked shared_ptrs are equal
            auto sp1 = wp1.lock();
            auto sp2 = wp2.lock();
            return sp1 == sp2;
        }
    };

    class Graph {
        std::unordered_map<Node *, std::shared_ptr<Node> > nodes;

    public:
        std::unordered_map<Pin *, std::unordered_set<std::weak_ptr<Node>, weak_ptr_hash<Node>, weak_ptr_equal<Node> > >
        outputLinks;

        Graph() : outputLinks({}) {
        }

        void connect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Node> &childNode,
                     unsigned int childIndex);

        void disconnect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Node> &childNode,
                        unsigned int childIndex);
    };
}


#endif //GRAPH_H
