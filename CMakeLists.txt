cmake_minimum_required(VERSION 3.15)
project(Gravity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -pthread -msse4.2 -march=native")
find_package(Threads REQUIRED)

INCLUDE_DIRECTORIES(/usr/local/opt/taskflow
        /usr/local/opt/entt/include
        /usr/local/opt/mathsimd
        /usr/local/opt/mtl_cpp/include)
add_definitions(-DSHADER_SOURCE="${CMAKE_SOURCE_DIR}/src/shader.metal")
add_executable(Gravity main.cpp
        src/BH_QuadTree.cpp
        src/BH_Tests.cpp
        include/BH_Tests.hpp
        src/World.cpp
        include/World.hpp
        include/components/components.hpp
        src/Camera.cpp
        include/Camera.hpp
        include/Mesh.hpp
        src/Mesh.cpp
        src/Renderer.cpp
        include/Renderer.hpp
        include/Utils.hpp)