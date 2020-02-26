

#ifndef GRAVITY_AABB_HPP
#define GRAVITY_AABB_HPP
#include <mathsimd.hpp>
struct AABB {
    mathsimd::float2 min;
    mathsimd::float2 max;
    AABB(float min_x, float min_y, float max_x, float max_y) : min(min_x,min_y), max(max_x, max_y) {}
    int quadrant_mask(mathsimd::float2 const &point, mathsimd::float2 &mid) {
        mid = min + (max - min) * 0.5f;
        auto tmp0 = static_cast<__m128>(point) < static_cast<__m128>(mid);

        return _mm_movemask_epi8(_mm_castps_si128(tmp0));
    }

};


#endif //GRAVITY_AABB_HPP
