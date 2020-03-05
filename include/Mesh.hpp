#ifndef GRAVITY_MESH_HPP
#define GRAVITY_MESH_HPP
#include <mathsimd.hpp>
#include <vector>
#include <cstdint>
namespace gravity {
    struct Mesh {
        std::vector<mathsimd::float2> vertices;
        std::vector<uint16_t> triangles;
        static Mesh makeCircle();
    };
}

#endif //GRAVITY_MESH_HPP
