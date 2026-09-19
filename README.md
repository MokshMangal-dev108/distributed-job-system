# Distributed Job System

A backend-oriented distributed job processing system built from scratch.

The goal of this project is to understand how real backend systems handle **networking, concurrency, thread pools, job queues, synchronization, and distributed task processing** by implementing the core components from scratch.

## Architecture

```text
                    TCP
Client  ──────────────────────────>  Server
                                      |
                                      v
                                JobManager
                                      |
                                      v
                                  JobQueue
                                      |
                                      v
                                 ThreadPool
                                      |
                         ┌────────────┼────────────┐
                         v            v            v
                      Worker 1     Worker 2     Worker 3
                         |
                         v
                    Job::execute()
```
## Supported Commands

The server currently supports:

```text
CREATE <job name>
GET <job id>
SUBMIT <job id>
LIST
```

Example:

```text
CREATE SEND_EMAIL
CREATED 0

GET 0
JOB 0 SEND_EMAIL

SUBMIT 0
SUBMITTED 0
```

## Project Structure

```text
DistributedJobSystem/
│
├── CMakeLists.txt
│
├── include/
│   ├── Job.h
│   ├── JobManager.h
│   ├── JobQueue.h
│   ├── ThreadPool.h
│   └── Server.h
│
├── src/
│   ├── main.cpp
│   ├── Job.cpp
│   ├── JobManager.cpp
│   ├── JobQueue.cpp
│   ├── ThreadPool.cpp
│   └── Server.cpp
│
└── tests/
```

## How It Works

### 1. Job Creation

A client sends:

```text
CREATE SEND_EMAIL
```

The server passes the request to `JobManager`.

`JobManager` creates and stores the job.

```text
Client
  |
  v
Server
  |
  v
JobManager
  |
  v
Job
```

### 2. Job Submission

A client sends:

```text
SUBMIT 0
```

The job is copied into the thread-safe `JobQueue`.

```text
JobManager
    |
    v
JobQueue
```

### 3. Worker Processing

Worker threads wait for jobs using a `condition_variable`.

When a job becomes available:

```text
JobQueue
    |
    v
Worker
    |
    v
Job::execute()
```

Multiple workers can process different jobs concurrently.

## Concurrency

The project uses C++ synchronization primitives to safely coordinate multiple threads.

### JobQueue

* `std::mutex` protects the queue.
* `std::condition_variable` puts workers to sleep when there are no jobs.
* `notify_one()` wakes a worker when a job arrives.
* `notify_all()` wakes workers during shutdown.

### JobManager

The job storage is protected using a mutex because multiple client threads can access it concurrently.

## Networking

The server uses **TCP sockets through Windows Winsock**.

The current server:

1. Initializes Winsock.
2. Creates a TCP socket.
3. Binds to port `8080`.
4. Starts listening.
5. Accepts clients.
6. Creates a thread for each client.
7. Processes commands received over TCP.

## Build

The project uses **CMake** and requires a C++20-compatible compiler.

On Windows, the project uses Winsock:

```text
ws2_32
```

CMake links the required networking library automatically on Windows.
