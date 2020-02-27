#include <iostream>
#include "include/BH_QuadTree.hpp"
#include <mathsimd.hpp>
#include <algorithm>
#include <chrono>
#include <random>
#include <thread>
#include <entt/entt.hpp>

constexpr int SEED = 2324314;
constexpr unsigned int VALUES = 10000u;
constexpr unsigned int TESTS = 10u;

static float rnd(float min = 0.f, float max = 1.f) {
    static std::default_random_engine engine(SEED);
    std::uniform_real_distribution d(min, max);
    return d(engine);
}

static std::array<mathsimd::float2,VALUES>& generate_positions() {
    static bool created = false;
    static std::array<mathsimd::float2,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = mathsimd::float2(rnd(-100,100),rnd(-100,100));
    }
    created = true;
    return test_cases;
}

static std::array<float,VALUES>& generate_masses() {
    static bool created = false;
    static std::array<float,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = rnd(0,200);
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

/* Coordinate HASH
 * constexpr float WORLD_WIDTH = 200;
    constexpr float N_HORIZONTAL = 4;
    constexpr float WORLD_HEIGHT = 200;
    constexpr float N_VERTICAL = 2;

    float x = 92.132f;
    float y = 23.234f;
    printf("%f\n", std::floor((x + WORLD_WIDTH * 0.5f)/(WORLD_WIDTH / N_HORIZONTAL)) + N_HORIZONTAL * std::floor((WORLD_HEIGHT * 0.5f - y)/(WORLD_HEIGHT / N_VERTICAL)) );
 *
 *
 * */

int main() {

    double time = 0.0;
    gravity::BH_QuadTree tree(0.025f, gravity::AABB(-100,-100,100,100), VALUES<<2u);
    for (int i = 0; i < TESTS; ++i) {
        time += test_qtree(tree);
        tree.clear();
    }
    printf("Time: %f us\n", time / TESTS);

    return 0;
}