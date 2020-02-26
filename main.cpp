#include <iostream>
#include "include/BH_QuadTree.hpp"
#include <mathsimd.hpp>
#include <immintrin.h>
#include <chrono>

constexpr int SEED = 2345;
constexpr unsigned int VALUES = 10000u;

static float rnd() {
    static int seed = SEED;
    seed = int(std::fmod(static_cast<float>(seed) * 1373.f + 691.f, 509.f));
    return static_cast<float>(seed) / 509.f;
}

static std::array<mathsimd::float2,VALUES>& generate_positions() {
    static bool created = false;
    static std::array<mathsimd::float2,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = mathsimd::float2((rnd()-0.5f)*200,(rnd()-0.5f)*200);
    }
    created = true;
    return test_cases;
}

static std::array<float,VALUES>& generate_masses() {
    static bool created = false;
    static std::array<float,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = rnd() * 200.f;
    }
    created = true;
    return test_cases;
}


int main() {
    using namespace gravity;
    auto pos = generate_positions();
    auto m = generate_masses();
    BH_QuadTree tree(0.04f, AABB(-250,-250,250,250), VALUES);
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < VALUES; ++i) {
        tree.insert(pos[i],m[i]);
    }
    auto elapsed = high_resolution_clock::now() - start;
    printf("Quad tree insertion Elapsed in us: %f\n", static_cast<double>(duration_cast<microseconds>(elapsed).count()));
    float val = 0;
    for (auto& i: tree.data) {
        val += i.mass;
    }
    printf("Total mass: %f\n", val);
    printf("Centre of mass: %f,%f\n", tree.data[0].centre.x(), tree.data[0].centre.y());

    return 0;
}