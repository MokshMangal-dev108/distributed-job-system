#include <iostream>
#include "../include/Job.h"


Job::Job(int id, const std::string& name)
    : id(id), name(name) {
}

int Job::getId() const {
    return id;
}

const std::string& Job::getName() const {
    return name;
}

void Job::execute() {

    std::cout
        << "Executing job: "
        << id
        << " - "
        << name
        << '\n';
}