#include "../include/Mesh.hpp"
#include <cstdint>
#include <mathsimd.hpp>


gravity::Mesh gravity::Mesh::makeCircle() {
    static constexpr float R = 0.5f;
    static constexpr uint16_t SIDES = 120;
    static constexpr float INV_2PI = 0.1591549431f;
    Mesh out;
    out.vertices.reserve(SIDES + 1);
    int v = 1;
    out.vertices.emplace_back(0,0,0);
    while (v <= SIDES) {
        float rad = static_cast<float>(v) / SIDES * INV_2PI;
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
