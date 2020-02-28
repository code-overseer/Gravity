#include <iostream>
#include "include/BH_QuadTree.hpp"
#include "include/BH_Grid.hpp"
#include <mathsimd.hpp>
#include <algorithm>
#include <chrono>
#include <random.hpp>
#include <thread>
#include <entt/entt.hpp>

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
    for (auto const&i : pos) {
        int idx = grid.getIndex(i);
        ++count[idx];
    }
    std::exclusive_scan(count.begin(), count.end(), count.begin(), 0);
    for (int i = 15; i >= 0; --i) {
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
    auto ends = group(pos, grid);
    double total;
    auto start = high_resolution_clock::now();

    std::thread threads[7];
    for (int i = 1; i < 8; ++i) {
        auto p = pos.begin() + ends[i-1];
        auto m = mass.begin() + ends[i-1];
        threads[i - 1] = std::thread(insertion, &grid.tree(i), p, m, ends[i] - ends[i - 1]);
    }
    insertion(&grid.tree(0), pos.begin(), mass.begin(), ends[0]);
    for (auto &th : threads) th.join();
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());

    return total;

}

int main() {
    Grid grid(WORLD, VALUES, 0.025f);
//    double time = 0.0;
//    gravity::BH_QuadTree tree(0.025f, WORLD, VALUES<<2u);
//    for (int i = 0; i < TESTS; ++i) {
//        time += test_grid(grid);
//        tree.clear();
//    }
//    printf("Time: %f us\n", time / TESTS);
    printf("Time: %f us\n", test_grid(grid));
    return 0;
}