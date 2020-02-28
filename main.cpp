#include <iostream>
#include "include/BH_QuadTree.hpp"
#include "include/BH_Grid.hpp"
#include <mathsimd.hpp>
#include <algorithm>
#include <chrono>
#include <random.hpp>
#include <entt/entt.hpp>
#include <taskflow.hpp>

constexpr int SEED = 2324314;
constexpr unsigned int VALUES = 10000u;
constexpr unsigned int TESTS = 1000u;
constexpr unsigned int GRID_H = 4u;
constexpr unsigned int GRID_V = 2u;
constexpr unsigned int N_CELLS = GRID_H*GRID_V;
const gravity::AABB WORLD(-100,-100,100,100); // WORLD
static mathsimd::Random r(SEED);
using Grid = gravity::BH_Grid<GRID_H, GRID_V>;

static std::array<mathsimd::float2,VALUES>& generate_positions() {
    static bool created = false;
    static std::array<mathsimd::float2,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = mathsimd::float2(r.rnd(WORLD.min.x(),WORLD.max.x()),r.rnd(WORLD.min.y(),WORLD.max.y()));
    }
    created = true;
    return test_cases;
}

static std::array<float,VALUES>& generate_masses() {
    static bool created = false;
    static std::array<float,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = r.rnd(0.f,200.f);
    }
    created = true;
    return test_cases;
}

double test_qtree(gravity::BH_QuadTree & tree) {
    using namespace gravity;
    auto pos = generate_positions();
    auto m = generate_masses();
    using namespace std::chrono;
    double total;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < VALUES; ++i) {
        tree.insert(pos[i], m[i]);
    }
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());

    float val = 0;
    for (auto& i: tree.data) { val += i.mass; }


    return total;
}

static std::array<int, N_CELLS> group(std::array<mathsimd::float2,VALUES>& pos, Grid const& grid) {
    std::array<int, N_CELLS> count{};
    memset(count.data(),0, N_CELLS*sizeof(int));
    using namespace std::chrono;
    for (auto const&i : pos) {
        int idx = grid.getIndex(i);
        ++count[idx];
    }

    std::exclusive_scan(count.begin(), count.end(), count.begin(), 0);
    for (int i = pos.size() - 1; i >= 0; --i) {
        auto& val = pos[i];
        auto key = grid.getIndex(val);
        int j = count[key];

        while (j < i) {
            ++count[key];
            std::swap(pos[j], val);
            key = grid.getIndex(val);
            j = count[key];
        }

        count[key] += (i == j);
    }
    return count;
}

void insertion(gravity::BH_QuadTree *tree, mathsimd::float2 const* pos, float const* mass, int n) {
    for (int i = 0; i < n; ++i) {
        tree->insert(pos[i], mass[i]);
    }
}

double test_grid(Grid & grid) {
    using namespace gravity;
    auto pos = generate_positions();
    auto mass = generate_masses();
    using namespace std::chrono;
    static tf::Executor executor;

    auto ends = group(pos, grid);

    double total;
    auto start = high_resolution_clock::now();
    tf::Taskflow taskflow;

    for (int i = 0; i < N_CELLS; ++i) {
        auto p = pos.begin() + ((i == 0) ? 0 : ends[i-1]);
        auto m = mass.begin() + ((i == 0) ? 0 : ends[i-1]);
        BH_QuadTree* tree = &grid.tree(i);
        auto l = ends[i] - ((i == 0) ? 0 : ends[i-1]);
        taskflow.emplace([tree,p,m,l]() { for (int i = 0; i < l; ++i) tree->insert(p[i],m[i]); });
    }

    executor.run(taskflow);
    executor.wait_for_all();
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());

    return total;

}

int main() {

    {
        gravity::BH_QuadTree tree(0.025f, WORLD, VALUES << 2u);
        double seq_time = 0.0;
        for (int i = 0; i < TESTS; ++i) {
            seq_time += test_qtree(tree);
            tree.clear();
        }
        printf("Sequential Time: %f us\n", seq_time / TESTS);
    }
    {
        Grid grid(WORLD, VALUES, 0.025f);
        double par_time = 0.0;
        for (int i = 0; i < TESTS; ++i) {
            par_time += test_grid(grid);
            grid.clear();
        }

        printf("Parallel Time: %f us\n", par_time / TESTS);

    }



    return 0;
}