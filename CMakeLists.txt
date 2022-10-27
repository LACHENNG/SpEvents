cmake_minimum_required(VERSION 3.0)

project (myMuduo C CXX)

find_package(Boost REQUIRED)
message(${Boost_INCLUDE_DIRS})


set(SRC 
    Eventloop.cpp
    Channel.cpp
    Poller.cpp
    )

add_executable(test1 
              test1.cpp
              ${SRC})

add_executable(test2
            test2.cpp
            ${SRC})

target_link_libraries(test1
    pthread
)
target_link_libraries(test2
    pthread
)

