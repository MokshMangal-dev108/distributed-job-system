#include <iostream>
#include "../include/Job.h"
#include "../include/JobManager.h"
#include "../include/JobQueue.h"
#include "../include/ThreadPool.h"
#include "../include/server.h"

int main() {

    JobQueue queue;

    ThreadPool pool(queue, 3);

    JobManager manager(queue);

    Server server(8080, manager);

    server.start();
    
    return 0;
}
