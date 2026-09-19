#ifndef DISTRIBUTEDJOBSYSTEM_JOB_H
#define DISTRIBUTEDJOBSYSTEM_JOB_H
#include <string>


class Job {
private:
    int id;
    std::string name;
public:
    Job(int id, const std::string &name);
    int getId() const;
    const std::string& getName() const;
    void execute();
};


#endif //DISTRIBUTEDJOBSYSTEM_JOB_H
