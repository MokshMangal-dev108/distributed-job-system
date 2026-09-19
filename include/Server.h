
#ifndef DISTRIBUTEDJOBSYSTEM_SERVER_H
#define DISTRIBUTEDJOBSYSTEM_SERVER_H
#include "JobManager.h"
#include <winsock2.h>
class Server {

private:
    int port;

    JobManager& jobManager;

    std::string handleCommand(const std::string& command);

    void handleClient(SOCKET clientSocket);

public:
    Server(int port, JobManager& jobManager);

    void start();
};


#endif //DISTRIBUTEDJOBSYSTEM_SERVER_H
