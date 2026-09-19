#include <iostream>
#include "../include/JobManager.h"

JobManager::JobManager(JobQueue& queue)
    : queue(queue) {
}

int JobManager::CreateJob(const std::string &name) {
    std::lock_guard<std::mutex> lock(mutex);
    int id = nextJobId++;

    Job job(id, name);

    jobs.emplace(id, job);
    return id;
}

bool JobManager::getJob(int id, Job &job) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = jobs.find(id);
    if (it == jobs.end()) {
        return false;
    }
    job = it->second;
    return true;
}

void JobManager::submitJob(int id) {

    Job job(0, "");

    bool found =
        getJob(id, job);

    if (!found) {
        std::cout<< "Cannot submit job. " << "Job not found: " << id << '\n';
        return;
    }

    queue.push(job);

    std::cout<< "Job " << id << " submitted to queue.\n";
}


void JobManager::printAllJobs() const {
    std::lock_guard<std::mutex> lock(mutex);
    if (jobs.empty()) {
        std::cout << "No jobs available.\n";
        return;
    }
    for (const auto& [id, job] : jobs) {
        std::cout
            << "Job ID: " << id
            << " | Name: " << job.getName()
            << '\n';
    }
}