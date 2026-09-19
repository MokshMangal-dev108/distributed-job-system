#include "../include/JobQueue.h"
#include <iostream>

void JobQueue::push(const Job &job) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (stopped) {
            return;
        }
        jobs.push(job);
    }
    condition.notify_one();
}

bool JobQueue::pop(Job& job) {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock,[this]() {
        return stopped || !jobs.empty();
    });

    if (stopped && jobs.empty()) {
        return false;
    }
    job = jobs.front();
    jobs.pop();
    return true;
}

void JobQueue::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        stopped = true;
    }
    condition.notify_all();
}