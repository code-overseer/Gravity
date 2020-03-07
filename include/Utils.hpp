#ifndef GRAVITY_UTILS_HPP
#define GRAVITY_UTILS_HPP
#include <string>
#include <fstream>
#include "AABB.hpp"

inline std::string readFile(char const* path) {
    std::ifstream file(path);
    if (file.fail()) { throw std::runtime_error( "Could not open file"); }
    std::string text;
    file.seekg(0, std::ios::end);
    text.reserve(((size_t)file.tellg()) + 1);
    file.seekg(0, std::ios::beg);
    text.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
    return text;
}

inline int getIndex(mathsimd::float2 p, gravity::AABB const& world, float const& width, float const& height) {
    using namespace mathsimd;
    p = p - world.min;
    p = p / (world.max - world.min);
    p = p * float2{width,height};
    p = float2(std::floor(p.x()),std::floor(p.y()));
    return static_cast<int>(dot(p, {1, width}));
}
#endif //GRAVITY_UTILS_HPP
