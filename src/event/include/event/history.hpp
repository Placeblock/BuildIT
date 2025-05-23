//
// Created by felix on 11.05.25.
//

#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <functional>
#include <memory>

namespace buildit::event {
template<typename E>
class history {
public:
    using event_type = E;

    virtual ~history() = default;

    virtual void receive(std::unique_ptr<E> event, const std::function<void()>& runHandlers) = 0;
};

template<typename T>
concept is_history = requires() { std::is_base_of_v<history<typename T::event_type>, T>; };

} // namespace buildit::event

#endif //HISTORY_HPP
