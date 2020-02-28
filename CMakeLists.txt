cmake_minimum_required(VERSION 3.15)
project(Gravity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -pthread -msse4.2 -march=native")
find_package(Threads REQUIRED)

INCLUDE_DIRECTORIES(/usr/local/opt/taskflow
        /usr/local/opt/entt/include
        /usr/local/opt/mathsimd)

add_executable(Gravity main.cpp src/BH_QuadTree.cpp)