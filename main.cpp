#include <iostream>
#include "include/BH_QuadTree.hpp"
#include <mathsimd.hpp>
#include <algorithm>
#include <chrono>
#include <random>

constexpr int SEED = 2324314;
constexpr unsigned int VALUES = 10000u;
constexpr unsigned int TESTS = 1000u;

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

double test_qtree() {
    using namespace gravity;
    auto pos = generate_positions();
    auto m = generate_masses();
    BH_QuadTree tree(0.025f, AABB(-100,-100,100,100), VALUES<<2u);
    using namespace std::chrono;
    double total;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < VALUES; ++i) {
        tree.insert(pos[i], m[i]);
    }
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());
//    printf("Quad tree insertion Elapsed in us: %f\n",  total);
    float val = 0;
    for (auto& i: tree.data) { val += i.mass; }
//    printf("%f ", val);
//    printf("Centre of mass: %f,%f\n", tree.data[0].centre.x(), tree.data[0].centre.y());
//    printf("vector length: %lu\n", tree.data.size());

    return total;
}


int main() {

    double time = 0.0;
    for (int i = 0; i < TESTS; ++i) {
        time += test_qtree();
    }
    printf("Time: %f us\n", time / TESTS);
    return 0;
}