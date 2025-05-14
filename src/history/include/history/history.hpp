//
// Created by felix on 11.05.25.
//

#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <functional>
#include <memory>

namespace History {
template <typename E>
class History {
public:
    virtual ~History() = default;

    virtual void receive(std::unique_ptr<E> event, const std::function<void()>& runHandlers) = 0;

};
} // namespace History

#endif //HISTORY_HPP
