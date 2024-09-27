//
// Created by felix on 9/25/24.
//

#ifndef BUILDIT_THREADPOOL_H
#define BUILDIT_THREADPOOL_H


#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

class ThreadPool {
public:
    explicit ThreadPool(uint threadCount, uint maxJobs = 0);
    void start();
    void queue(const std::function<void()>& job);
    void waitDone();
    [[nodiscard]] uint getThreadCount() const;
private:
    const uint maxJobs;
    const uint threadCount;
    std::mutex queueMutex;
    std::condition_variable newJobCondition;
    std::condition_variable processingJobCondition;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;

    std::mutex runningJobsMutex;
    std::condition_variable_any jobsDoneCondition;
    std::atomic<int> runningJobs = 0;

    [[noreturn]] void threadLoop();
};


#endif //BUILDIT_THREADPOOL_H
