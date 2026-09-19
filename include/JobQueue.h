//
// Created by Dell on 19-09-2026.
//
#include <condition_variable>

#include "Job.h"
#include <iostream>
#include <mutex>
#include <queue>
#ifndef DISTRIBUTEDJOBSYSTEM_JOBQUEUE_H
#define DISTRIBUTEDJOBSYSTEM_JOBQUEUE_H

class JobQueue {
private:
    std::queue<Job>jobs;
    std::mutex mutex;
    std::condition_variable condition;
    bool stopped = false;
public:
    void push(const Job &job);
    bool pop(Job& job);
    void stop();
};

#endif //DISTRIBUTEDJOBSYSTEM_JOBQUEUE_H
