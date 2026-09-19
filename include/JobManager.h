//
// Created by Dell on 19-09-2026.
//

#ifndef DISTRIBUTEDJOBSYSTEM_JOBMANAGER_H
#define DISTRIBUTEDJOBSYSTEM_JOBMANAGER_H
#include <unordered_map>
#include "../include/JobQueue.h"
#include "Job.h"


class JobManager {
private:
    int nextJobId=0;
    std::unordered_map<int, Job> jobs;
    JobQueue &queue;
    mutable  std::mutex mutex;

public:
    explicit JobManager(JobQueue& queue);
    int CreateJob(const std::string& name);
    bool getJob(int id, Job &job);
    void printAllJobs() const;
    void submitJob(int id);
};

#endif //DISTRIBUTEDJOBSYSTEM_JOBMANAGER_H
