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
    p = mathsimd::float2::minimum(p, world.max * 0.9999f);
    p = mathsimd::float2::maximum(p, world.min * 1.0001f);

    __m128 wm = world.min;
    __m128 wma = world.max;
    __m128 pa = p;
    __m128 wh{width, height};
    pa =  _mm_mul_ps(_mm_floor_ps((pa - wm) / (wma - wm) * wh), __m128{1,width});
    return static_cast<int>(pa[0] + pa[1]);
}
#endif //GRAVITY_UTILS_HPP
