#ifndef GRAVITY_BH_GRID_HPP
#define GRAVITY_BH_GRID_HPP

#include "AABB.hpp"
#include "BH_QuadTree.hpp"

namespace gravity {
    /* Grid index goes from left to right, bottom to top */
    template <int WIDTH, int HEIGHT>
    struct BH_Grid {
        static constexpr int N = WIDTH * HEIGHT;
    private:
        std::vector<BH_QuadTree> trees_;
    public:
        AABB const WORLD;
        BH_Grid(AABB bounds, unsigned int entities, float theta_sqr) : WORLD(std::move(bounds)) {
            using namespace mathsimd;
            float2 step = (WORLD.max - WORLD.min);
            step.x() /= WIDTH;
            step.y() /= HEIGHT;
            trees_.reserve(N);
            for (int i = 0; i < N; ++i) {
                float2 min = WORLD.min + float2{static_cast<float>(i % WIDTH),static_cast<float>(i / WIDTH)} * step;
                trees_.emplace_back(theta_sqr, AABB{min, min + step}, (entities / N) << 2u);
            }
        }

        [[nodiscard]] BH_QuadTree const& tree(int idx) const { return trees_[idx]; }
        BH_QuadTree& tree(int idx) { return trees_[idx]; }
        void clear() {
            for (auto &i : trees_) i.clear();
        }

    };
}



#endif //GRAVITY_BH_GRID_HPP
