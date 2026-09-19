cmake_minimum_required(VERSION 4.3)
project(DistributedJobSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(DistributedJobSystem src/main.cpp
        include/Job.h
        src/Job.cpp
        include/JobManager.h
        src/JobManager.cpp
        include/JobQueue.h
        src/JobQueue.cpp
        include/ThreadPool.h
        src/ThreadPool.cpp
        include/Server.h
        src/Server.cpp)


if (WIN32)
    target_link_libraries(
            DistributedJobSystem
            PRIVATE
            ws2_32
    )
endif ()