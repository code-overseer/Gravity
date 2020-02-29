#include <iostream>
#include "include/BH_Tests.hpp"
#include <entt/entt.hpp>
#include <taskflow.hpp>

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

int main() {



    return 0;
}