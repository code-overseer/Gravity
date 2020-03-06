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
static gravity::Renderer r = gravity::Renderer();
int main() {
    using namespace std::chrono;
    gravity::World w;
    w.initializeParticles();
    launch_app();
    bool u = true;
    auto now = std::chrono::high_resolution_clock::now();
    while (u) {
        w.update();
        auto elapsed = high_resolution_clock::now() - now;
        if (static_cast<double>(duration_cast<microseconds>(elapsed).count())*1000 < 16.6) continue;
        now = std::chrono::high_resolution_clock::now();
        w.preDraw(r);
        update_view(&u);
    }


    return 0;
}