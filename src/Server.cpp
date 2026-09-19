#include <iostream>
#include "../include/Server.h"
#include <winsock2.h>
#include <ws2tcpip.h>

Server::Server(int port, JobManager& jobManager) : port(port), jobManager(jobManager) {

}

std::string Server::handleCommand( const std::string& command) {
    // CREATE <job name>

    if (command.rfind("CREATE ", 0) == 0) {

        std::string jobName =
            command.substr(7);

        if (jobName.empty()) {
            return "ERROR Job name is empty";
        }
        int jobId =
            jobManager.CreateJob(jobName);

        return "CREATED " + std::to_string(jobId);
    }
    // ============================================
    // GET <job id>
    // ============================================

    if (command.rfind("GET ", 0) == 0) {

        std::string idString =
            command.substr(4);

        try {

            int jobId = std::stoi(idString);

            Job job(0, "");

            bool found = jobManager.getJob(jobId, job);

            if (!found) {
                return "ERROR Job not found";
            }

            return "JOB " + std::to_string(job.getId()) + " " + job.getName();

        } catch (...) {
            return "ERROR Invalid job ID";
        }
    }

    // ============================================
    // SUBMIT <job id>
    // ============================================

    if (command.rfind("SUBMIT ", 0) == 0) {

        std::string idString =
            command.substr(7);

        try {

            int jobId =
                std::stoi(idString);

            jobManager.submitJob(jobId);

            return "SUBMITTED " +
                   std::to_string(jobId);

        } catch (...) {

            return "ERROR Invalid job ID";
        }
    }

    // ============================================
    // LIST
    // ============================================

    if (command == "LIST") {

        jobManager.printAllJobs();

        return "OK LIST";
    }

    return "ERROR Unknown command";
}

void Server::handleClient(SOCKET clientSocket) {

    std::cout<< "Client handler started.\n";
    char buffer[1024];

    while (true) {

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

        // Client disconnected
        if (bytesReceived == 0) {
            std::cout << "Client disconnected.\n";
            break;
        }

        // Error
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recv() failed. Error code: " << WSAGetLastError() << '\n';
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string command(buffer);

        while (!command.empty() && (command.back() == '\n' || command.back() == '\r')) {
            command.pop_back();
        }

        std::cout<< "Received: "<< command << '\n';

        // Process command
        std::string response = handleCommand(command);

        std::cout<< "Response: " << response << '\n';

        // Send response

        response += "\n";

        int bytesSent = send(clientSocket, response.c_str(), static_cast<int>(response.size()),0);

        if (bytesSent == SOCKET_ERROR) {
            std::cerr<< "send() failed.\n";
            break;
        }
    }
    closesocket(clientSocket);
    std::cout << "Client handler stopped.\n";
}


void Server::start() {

    std::cout
        << "Starting server on port "
        << port
        << "...\n";

    // 1. Initialize Winsock
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        std::cerr
            << "WSAStartup failed: "
            << result
            << '\n';
        return;
    }
    std::cout << "Winsock initialized.\n";

    // 2. Create socket

    SOCKET serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return;
    }

    std::cout << "Socket created.\n";

    // 3. Configure server address

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;

    serverAddress.sin_addr.s_addr = INADDR_ANY;

    serverAddress.sin_port = htons(port);

    // 4. Bind socket

    result = bind(
        serverSocket,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)
    );

    if (result == SOCKET_ERROR) {

        std::cerr
            << "Bind failed.\n";

        closesocket(serverSocket);
        WSACleanup();

        return;
    }
    std::cout << "Socket bound.\n";

    result = listen(
        serverSocket,
        SOMAXCONN
    );

    if (result == SOCKET_ERROR) {

        std::cerr
            << "Listen failed.\n";

        closesocket(serverSocket);
        WSACleanup();

        return;
    }

    std::cout
        << "Server listening on port "
        << port
        << "...\n";


    // 6. Accept client
    while (true) {
        SOCKET clientSocket = accept(
            serverSocket,
            nullptr,
            nullptr
        );

        if (clientSocket == INVALID_SOCKET) {
            std::cerr<< "Accept failed.\n";
            continue;
        }

        std::cout<< "Client connected!\n";

        // Create a thread for this client

        std::thread clientThread(
            &Server::handleClient,
            this,
            clientSocket
        );

        // Let the thread run independently
        clientThread.detach();
    }

    closesocket(serverSocket);

    WSACleanup();

    std::cout << "Server stopped.\n";

}
