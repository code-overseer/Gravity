#ifndef GRAVITY_BH_GRID_HPP
#define GRAVITY_BH_GRID_HPP

#include "AABB.hpp"
#include "BH_QuadTree.hpp"

namespace gravity {
    /* Grid index goes from left to right, bottom to top */
    template <int WIDTH, int HEIGHT>
    struct BH_Grid {
        static constexpr const int TagBase = 1000;
        static constexpr const int W = WIDTH;
        static constexpr const int H = HEIGHT;
        static constexpr const int N = WIDTH * HEIGHT;
        static constexpr int CellTag(int idx) {
            return TagBase + idx;
        }
    private:
        std::vector<BH_QuadTree> trees_;
    public:
        AABB const Bound;
        mathsimd::float2 CellSize;
        BH_Grid(AABB bounds, unsigned int entities, float theta_sqr) : Bound(std::move(bounds)) {
            using namespace mathsimd;
            CellSize = (Bound.max - Bound.min) / mathsimd::float2(WIDTH, HEIGHT);
            trees_.reserve(N);
            for (int i = 0; i < N; ++i) {
                float2 min = Bound.min + float2{static_cast<float>(i % WIDTH), static_cast<float>(i / WIDTH)} * CellSize;
                trees_.emplace_back(theta_sqr, AABB{min, min + CellSize}, (entities / N) << 2u);
            }
        }

        inline decltype(auto) begin() { return trees_.begin(); }
        inline decltype(auto) end() { return trees_.end(); }

        [[nodiscard]] BH_QuadTree const& tree(int idx) const { return trees_[idx]; }
        BH_QuadTree& tree(int idx) { return trees_[idx]; }
        void clear() {
            for (auto &i : trees_) i.clear();
        }

    };
}



#endif //GRAVITY_BH_GRID_HPP
