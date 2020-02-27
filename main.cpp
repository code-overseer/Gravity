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

static void sort_quadrant(std::array<mathsimd::float2,VALUES> &pos) {
    AABB mid(-100,-100,100,100);
    std::sort(pos.begin(), pos.end(), [&mid](mathsimd::float2 const&a, mathsimd::float2 const&b) {
       return mid.quadrant_index(a).first < mid.quadrant_index(b).first;
    });
}

double test_sorted_qtree() {
    using namespace gravity;
    auto pos = generate_positions();
    auto m = generate_masses();
    BH_QuadTree tree(0.025f, AABB(-100,-100,100,100), VALUES<<2u);
    using namespace std::chrono;
    double total;
    auto start = high_resolution_clock::now();
    sort_quadrant(pos);
    for (int i = 0; i < VALUES; ++i) {
        tree.insert(pos[i], m[i]);
    }
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());

    float val = 0;
    for (auto& i: tree.data) { val += i.mass; }

    return total;
}

static std::tuple<int,int,int> get_bins(std::array<mathsimd::float2,VALUES> &pos) {
    AABB mid(-100,-100,100,100);
    std::tuple<int,int,int> out {-1,-1,-1};
    for (int i = 0; i < pos.size(); ++i) {
        bool b0 = (mid.quadrant_index(pos[i]).first == 1 && std::get<0>(out) == -1);
        bool b1 = (mid.quadrant_index(pos[i]).first == 2 && std::get<1>(out) == -1);
        bool b2 = (mid.quadrant_index(pos[i]).first == 3 && std::get<2>(out) == -1);
        std::get<0>(out) = std::get<0>(out) * !b0 + b0 * i;
        std::get<1>(out) = std::get<1>(out) * !b1 + b1 * i;
        std::get<2>(out) = std::get<2>(out) * !b2 + b2 * i;
    }
    return out;
}


double test_parallel_sorted_qtree() {
    using namespace gravity;
    auto pos = generate_positions();
    auto m = generate_masses();
    BH_QuadTree tree[4]{{0.025f, AABB(-100,0,0,100), VALUES},
                        {0.025f, AABB(0,0,100,100), VALUES},
                        {0.025f, AABB(-100,-100,0,0), VALUES},
                        {0.025f, AABB(0,-100,100,0), VALUES}};
    std::thread threads[4];
    using namespace std::chrono;
    static const auto l = [] (mathsimd::float2* begin, mathsimd::float2* end, float const* mass, BH_QuadTree *bh) {
        for (auto i = begin; i != end; ++i, ++mass) { bh->insert(*i, *mass); }
    };
    double total;
    auto start = high_resolution_clock::now();
    sort_quadrant(pos);

    auto tup = get_bins(pos);

    threads[0] = std::thread(l, pos.begin(), pos.begin() + std::get<0>(tup), m.begin(), tree);
    threads[1] = std::thread(l, pos.begin() + std::get<0>(tup), pos.begin() + std::get<1>(tup), m.begin() + std::get<0>(tup), tree + 1);
    threads[2] = std::thread(l, pos.begin() + std::get<1>(tup), pos.begin() + std::get<2>(tup), m.begin() + std::get<1>(tup), tree + 2);
    threads[3] = std::thread(l, pos.begin() + std::get<2>(tup), pos.end(), m.begin() + std::get<2>(tup), tree + 3);
    for (auto &th : threads) th.join();
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());

    float val = 0;
    for (auto& t : tree) {
        for (auto& i: t.data) { val += i.mass; }
    }


    return total;
}

int main() {

    double time = 0.0;
    for (int i = 0; i < TESTS; ++i) {
        time += test_parallel_sorted_qtree();
    }
    printf("Time: %f us\n", time / TESTS);
//    AABB box(-100,-100,100,100);
//    mathsimd::float2 pos[4]{{1,1},{-1,-1},{-1,1},{1,-1}};
//    printf("Masks: %#02x,%#02x,%#02x,%#02x", box.quadrant_mask(pos[0]),box.quadrant_mask(pos[1]),box.quadrant_mask(pos[2]),box.quadrant_mask(pos[3]));
    return 0;
}