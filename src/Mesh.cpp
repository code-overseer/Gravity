#include "../include/Mesh.hpp"
#include <cstdint>
#include <mathsimd.hpp>


gravity::Mesh &gravity::Mesh::operator=(const gravity::Mesh &mesh) {
    if (&mesh == this) return *this;
    vertices = mesh.vertices;
    triangles = mesh.triangles;
    return *this;;
}

gravity::Mesh &gravity::Mesh::operator=(gravity::Mesh &&mesh) noexcept {
    if (&mesh == this) return *this;
    vertices = std::move(mesh.vertices);
    triangles = std::move(mesh.triangles);
    return *this;;
}

gravity::Mesh gravity::Mesh::makeCircle() {
    static constexpr float R = 0.5f;
    static constexpr uint16_t SIDES = 90;
    static constexpr float _2PI = 6.2831853072f;
    Mesh out;
    out.vertices.reserve(SIDES + 1);
    int v = 1;
    out.vertices.emplace_back(0,0);
    while (v <= SIDES) {
        float rad = static_cast<float>(v) / SIDES * _2PI;

        out.vertices.emplace_back(R * cos(rad), R * sin(rad));
        ++v;
    }
    out.triangles.reserve(3 * SIDES);

    for (uint16_t t = 0; t < SIDES; ++t) {
        out.triangles.emplace_back(0);
        out.triangles.emplace_back(t + 1);
        out.triangles.emplace_back((t == (SIDES-1)) ? 1 : t + 2);
    }

    return out;
}

