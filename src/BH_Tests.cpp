#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"


#include "../include/BH_Tests.hpp"
using namespace gravity;

AABB gravity::BH_Tests::WORLD(-1000,-1000,1000,1000);
mathsimd::Random gravity::BH_Tests::rand(SEED);

std::array<mathsimd::float2, BH_Tests::VALUES> &gravity::BH_Tests::generate_positions() {
    static bool created = false;
    static std::array<mathsimd::float2,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = mathsimd::float2(rand.rnd(WORLD.min.x(), WORLD.max.x()), rand.rnd(WORLD.min.y(), WORLD.max.y()));
    }
    created = true;
    return test_cases;
}

std::array<float, BH_Tests::VALUES> &gravity::BH_Tests::generate_masses() {
    static bool created = false;
    static std::array<float,VALUES> test_cases;
    if (created) return test_cases;
    for (auto &val : test_cases) {
        val = rand.rnd(0.f, 200.f);
    }
    created = true;
    return test_cases;
}

std::array<int, BH_Tests::N_CELLS>
gravity::BH_Tests::group(std::array<mathsimd::float2, VALUES> &pos, const gravity::BH_Tests::Grid &grid) {
    std::array<int, N_CELLS> count{};
    memset(count.data(),0, N_CELLS*sizeof(int));

    for (auto const&i : pos) {
        int idx = grid.getIndex(i);
        ++count[idx];
    }
    static std::array<mathsimd::float2,VALUES> COPY;
    std::exclusive_scan(count.begin(), count.end(), count.begin(), 0);
    for (auto const&i : pos) {
        auto k = grid.getIndex(i);;
        COPY[count[k]++] = i;
    }
    pos = COPY;

    return count;
}

double BH_Tests::test_qtree(gravity::BH_QuadTree &tree) {
    using namespace std::chrono;
    auto pos = generate_positions();
    auto m = generate_masses();
    double total;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < VALUES; ++i) {
        tree.insert(pos[i], m[i]);
    }
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());

    return total;
}

double BH_Tests::test_grid(BH_Tests::Grid &grid) {
    auto pos = generate_positions();
    auto mass = generate_masses();
    using namespace std::chrono;
    static tf::Executor executor;
    static tf::Taskflow taskflow;
    double total;
    auto start = high_resolution_clock::now();
    auto ends = group(pos, grid);

    for (int i = 0; i < N_CELLS; ++i) {
        auto p = pos.begin() + ((i == 0) ? 0 : ends[i-1]);
        auto m = mass.begin() + ((i == 0) ? 0 : ends[i-1]);
        BH_QuadTree* tree = &grid.tree(i);
        auto l = ends[i] - ((i == 0) ? 0 : ends[i-1]);
        taskflow.emplace([tree,p,m,l]() { for (int i = 0; i < l; ++i) tree->insert(p[i],m[i]); });
    }

    executor.run(taskflow);
    executor.wait_for_all();
    taskflow.clear();
    auto elapsed = high_resolution_clock::now() - start;
    total = static_cast<double>(duration_cast<microseconds>(elapsed).count());

    return total;
}

#pragma clang diagnostic pop