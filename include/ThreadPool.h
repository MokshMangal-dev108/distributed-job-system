
#ifndef DISTRIBUTEDJOBSYSTEM_THREADPOOL_H
#define DISTRIBUTEDJOBSYSTEM_THREADPOOL_H
#include "JobQueue.h"

class ThreadPool {
private:
    JobQueue& queue;
    std::vector<std::thread> workers;
    int numberOfThreads;

public:
    ThreadPool(JobQueue& queue, int numberOfThreads);
    ~ThreadPool();
    void stop();
};

#endif //DISTRIBUTEDJOBSYSTEM_THREADPOOL_H
