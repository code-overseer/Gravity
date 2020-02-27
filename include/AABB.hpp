

#ifndef GRAVITY_AABB_HPP
#define GRAVITY_AABB_HPP
#include <mathsimd.hpp>
#include <utility>
struct AABB {
    mathsimd::float2 min{0,0};
    mathsimd::float2 max{0,0};
    AABB(float min_x, float min_y, float max_x, float max_y) : min(min_x,min_y), max(max_x, max_y) {}
    AABB() = default;
    int quadrant_mask(mathsimd::float2 const &point, mathsimd::float2 &mid) {
        mid = min + (max - min) * 0.5f;
        auto tmp0 = static_cast<__m128>(point) < static_cast<__m128>(mid);

        return _mm_movemask_epi8(_mm_castps_si128(tmp0));
    }

    std::pair<int, AABB> quadrant_index(mathsimd::float2 const& point) {
        std::pair<int, AABB> output;
        output.second = *this;
        auto mid = min + (max - min) * 0.5f;
        auto tmp0 = static_cast<__m128>(point) < static_cast<__m128>(mid);
        switch (_mm_movemask_epi8(_mm_castps_si128(tmp0))) {
            case 0xff:
                output.first = 2;
                output.second.max = mid;
                break;
            case 0x00:
                output.first = 1;
                output.second.min = mid;
                break;
            case 0xf0:
                output.first = 3;
                output.second.min.x() = mid.x();
                output.second.max.y() = mid.y();
                break;
            case 0x0f:
                output.first = 0;
                output.second.min.y() = mid.y();
                output.second.max.x() = mid.x();
                break;
        }
        return output;
    }

};


#endif //GRAVITY_AABB_HPP
