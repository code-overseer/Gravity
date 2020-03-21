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

static void runApp() {
    using namespace std::chrono;
    gravity::World::Default();
    launch_app();
    bool u = true;
    auto timer = high_resolution_clock::now();
    double event_time = 0, predraw_time = 0, render_time = 0;
    bool rendered = false;

    while (u) {
        gravity::World::Default().update(static_cast<float>(rendered) * static_cast<float>(predraw_time + event_time + render_time));

        rendered = duration<double>(high_resolution_clock::now() - timer).count() > 0.0166;
        if (!rendered) continue;
        timer = high_resolution_clock::now();

        predraw_time = gravity::World::Default().predraw(static_cast<float>(event_time + render_time));

        auto t = high_resolution_clock::now();
        process_event(&u);
        event_time = duration<double>(high_resolution_clock::now() - t).count();

        t = high_resolution_clock::now();
        update_view(&u);
        render_time = duration<double>(high_resolution_clock::now() - t).count();

    }
}

int main() {

    runApp();
    return 0;
}