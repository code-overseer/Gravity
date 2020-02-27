

#ifndef GRAVITY_AABB_HPP
#define GRAVITY_AABB_HPP
#include <mathsimd.hpp>
#include <utility>
#include <iostream>
namespace gravity {
    struct AABB {
        mathsimd::float2 min{0,0};
        mathsimd::float2 max{0,0};
        AABB(float min_x, float min_y, float max_x, float max_y) : min(min_x,min_y), max(max_x, max_y) {}
        AABB(mathsimd::float2 const& min, mathsimd::float2 const& max) : min(min), max(max) {}
        AABB() = default;
        [[nodiscard]] inline int quadrant_idx(mathsimd::float2 const &point, mathsimd::float2 const &mid) const {
            auto tmp0 = static_cast<__m128>(point) < static_cast<__m128>(mid);
            return _mm_movemask_ps(tmp0);
        }
        [[nodiscard]] AABB build_quadrant(int idx, mathsimd::float2 const &mid) const {
            switch (idx) {
                case 3:
                    return {min, mid};
                case 0:
                    return {mid, max};
                case 2:
                    return {{mid.x(),min.y()}, {max.x(), mid.y()}};
                case 1:
                    return {{min.x(),mid.y()}, {mid.x(), max.y()}};
                default:
                    throw std::invalid_argument("idx only ranges from 0 to 3");
            }
        }
        [[nodiscard]] inline bool contains(mathsimd::float2 const &point) const {
            auto p = static_cast<__m128>(point);
            return _mm_movemask_ps(_mm_and_ps(p <= max,p >= min)) == 15;
        }

        [[nodiscard]] inline mathsimd::float2 centre() const { return min + 0.5f * (max - min); }
    };
}


#endif //GRAVITY_AABB_HPP
