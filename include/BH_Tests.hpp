

#ifndef GRAVITY_BH_TESTS_HPP
#define GRAVITY_BH_TESTS_HPP
#include "BH_Grid.hpp"
#include "BH_QuadTree.hpp"
#include "CollisionGrid.hpp"
#include "components.hpp"
#include <mathsimd.hpp>
#include <algorithm>
#include <chrono>
#include <random.hpp>
#include <entt/entt.hpp>
#include <taskflow.hpp>

namespace gravity {

    struct BH_Tests {
        static constexpr int SEED = 7532;
        static constexpr unsigned int VALUES = 10000u;
        static constexpr unsigned int TESTS = 1000u;
        static constexpr unsigned int GRID_H = 4u;
        static constexpr unsigned int GRID_V = 2u;
        static constexpr unsigned int N_CELLS = GRID_H * GRID_V;

        using Grid = gravity::BH_Grid<GRID_H, GRID_V>;
        static AABB WORLD;//(-1000,-1000,1000,1000);
        static mathsimd::Random rand;//(SEED);
        static std::array<mathsimd::float2,VALUES>& generate_positions();
        static std::array<gravity::components::CircleCollider, VALUES>&generate_colliders();
        static std::array<float,VALUES>& generate_masses();
        static std::array<int, N_CELLS> group(std::array<mathsimd::float2,VALUES>& pos, Grid const& grid);
        static double test_qtree(gravity::BH_QuadTree & tree);
        static double test_grid(Grid & grid);
        static double test_cgrid(CollisionGrid & grid, entt::registry &reg);
    };

}
#endif //GRAVITY_BH_TESTS_HPP
