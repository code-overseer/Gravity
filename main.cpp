#include <iostream>
#include "include/BH_Tests.hpp"
#include <entt/entt.hpp>
#include <taskflow.hpp>
#include "include/World.hpp"
#include <chrono>
extern "C" {
#include <Cocoa_API.h>
}
static void tests() {
    using namespace gravity;
    {
        BH_QuadTree tree(0.025f, BH_Tests::WORLD, BH_Tests::VALUES << 2u);
        double seq_time = 0.0;
        for (int i = 0; i < BH_Tests::TESTS; ++i) {
            seq_time += BH_Tests::test_qtree(tree);
            tree.clear();
        }
        printf("Sequential Time: %f us\n", seq_time / BH_Tests::TESTS);
    }
    {
        BH_Tests::Grid grid(BH_Tests::WORLD, BH_Tests::VALUES, 0.025f);
        double par_time = 0.0;
        for (int i = 0; i < BH_Tests::TESTS; ++i) {
            par_time += BH_Tests::test_grid(grid);
            grid.clear();
        }
        printf("Parallel Time: %f us\n", par_time / BH_Tests::TESTS);

    }
}

static void runApp() {
    using namespace std::chrono;
    static gravity::Renderer r = gravity::Renderer();
    launch_app();
    bool u = true;
    auto now = high_resolution_clock::now();
    while (u) {
        gravity::World::Update();
        if (duration<double>(high_resolution_clock::now() - now).count() < 0.0166) continue;
        now = high_resolution_clock::now();
        gravity::World::PreDraw(r);
        update_view(&u);
    }
}

static void test_collision() {
    using namespace gravity;
    auto pos = BH_Tests::generate_positions();
    auto col = BH_Tests::generate_colliders();
    std::vector<entt::entity> entities(pos.size());
    entt::registry reg;
    reg.create(entities.begin(), entities.end());
    reg.assign<mathsimd::float2>(entities.begin(),entities.end(), pos.begin());
    reg.assign<components::CircleCollider>(entities.begin(),entities.end(), col.begin());
    CollisionGrid grid(BH_Tests::WORLD, 200,200, BH_Tests::VALUES);

    double seq_time = 0.0;
    for (int i = 0; i < BH_Tests::TESTS; ++i) {
        seq_time += BH_Tests::test_cgrid(grid, reg);
        grid.clear();
    }
    printf("Sequential Time: %f us\n", seq_time / BH_Tests::TESTS);
}

int main() {

    runApp();
    return 0;
}