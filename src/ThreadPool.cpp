//
// Created by Dell on 19-09-2026.
//
#include "../include/ThreadPool.h"
ThreadPool::ThreadPool(JobQueue& queue, int numberOfThreads) : queue(queue){

    workers.reserve(numberOfThreads);
    for (int i = 0; i < numberOfThreads; i++) {
        workers.emplace_back([this, i] () {
            std::cout
               << "Worker "
               << i
               << " started\n";

            while (true) {
                Job job(0, "");
                bool success = this->queue.pop(job);

                if (!success) {
                    break;
                }

                std::cout
                    << "Worker "
                    << i
                    << " processing job\n";

                job.execute();
            }

            std::cout
                << "Worker "
                << i
                << " stopped\n";
        });
    }
};

void ThreadPool::stop() {
    queue.stop();
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

ThreadPool::~ThreadPool() {
    stop();
}
