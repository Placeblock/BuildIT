//
// Created by felix on 9/25/24.
//

#ifndef BUILDIT_UNIQUEBLOCKINGQUEUE_H
#define BUILDIT_UNIQUEBLOCKINGQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_set>

template<typename T>
class UniqueBlockingQueue {
private:
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<T> queue;
    std::unordered_set<T> items;
public:
    void push(const T& value) {
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            if (this->items.contains(value)) return;
            this->items.insert(value);
            queue.push(value);
        }
        this->condition.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->condition.wait(lock, [this]{ return !this->queue.empty(); });
        T rc(std::move(this->queue.back()));
        this->queue.pop();
        this->items.erase(rc);
        return rc;
    }
};


#endif //BUILDIT_UNIQUEBLOCKINGQUEUE_H
