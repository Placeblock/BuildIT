//
// Created by felix on 9/25/24.
//

#include <iostream>
#include "threadPool.h"

ThreadPool::ThreadPool(uint threadCount, uint maxJobs) : maxJobs(maxJobs), threadCount(threadCount) {

}

void ThreadPool::start() {
    for (size_t i = 0; i < this->threadCount; ++i) {
        threads.emplace_back(&ThreadPool::threadLoop, this);
    }
}

[[noreturn]] void ThreadPool::threadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(this->queueMutex);
            this->newJobCondition.wait(lock, [this] {
                return !jobs.empty();
            });
            std::unique_lock<std::mutex> lock2(this->runningJobsMutex);
            job = this->jobs.front();
            this->jobs.pop();
            this->runningJobs++;
        }
        processingJobCondition.notify_all();
        job();
        {
            std::unique_lock<std::mutex> lock(this->runningJobsMutex);
            this->runningJobs--;
        }
        this->jobsDoneCondition.notify_all();
    }
}

void ThreadPool::queue(const std::function<void()> &job) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        while(jobs.size() >= this->maxJobs && this->maxJobs != 0) {
            processingJobCondition.wait(lock);
        }
        jobs.push(job);
    }
    newJobCondition.notify_one();
}

void ThreadPool::waitDone() {
    std::unique_lock<std::mutex> lock(this->runningJobsMutex);
    if (this->runningJobs == 0 && this->jobs.empty()) return;
    this->jobsDoneCondition.wait(lock, [this] {
        return this->runningJobs == 0 && this->jobs.empty();
    });
}

uint ThreadPool::getThreadCount() const {
    return this->threadCount;
}
