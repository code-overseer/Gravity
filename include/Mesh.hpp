#ifndef GRAVITY_MESH_HPP
#define GRAVITY_MESH_HPP
#include <mathsimd.hpp>
#include <vector>
#include <cstdint>
namespace gravity {
    struct Mesh {
        Mesh() = default;
        Mesh(Mesh const&mesh) = default;
        Mesh(Mesh &&mesh) noexcept : vertices(std::move(mesh.vertices)), triangles(std::move(mesh.triangles)) {}
        Mesh& operator=(Mesh const&mesh);
        Mesh& operator=(Mesh &&mesh) noexcept ;
        std::vector<mathsimd::float2> vertices;
        std::vector<uint16_t> triangles;
        static Mesh makeCircle();
    };
}

#endif //GRAVITY_MESH_HPP
