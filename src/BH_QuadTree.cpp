

#include "../include/BH_QuadTree.hpp"

void gravity::BH_QuadTree::insert(const mathsimd::float2& pos, float mass)  {

    int idx = 0;
    AABB aabb = Root_box;
    while (nodes[idx].first_child > 0) {
        data[idx].mass += mass;
        auto tmp = mass / data[idx].mass;
        data[idx].centre = (data[idx].centre * (1 - tmp)) + (tmp * pos);
        auto mid = aabb.centre();
        auto n = aabb.quadrant_idx(pos, mid);
        aabb = aabb.build_quadrant(n, mid);
        idx = nodes[idx].first_child + n;
    }
    if (data[idx].isEmpty()) {
        data[idx].mass.val = mass;
        data[idx].centre = pos;
        return;
    }
    static std::vector<int> update;
    int n = idx,o = idx;
    while (n == o) {
        update.emplace_back(n);
        auto mid = aabb.centre();
        nodes[n].first_child = nodes.size();
        for (int i = 0; i < 4; ++i) nodes.emplace_back();
        for (int i = 0; i < 4; ++i) data.emplace_back();
        o = nodes[n].first_child + aabb.quadrant_idx(data[idx].centre, mid);
        auto t =aabb.quadrant_idx(pos, mid);
        n = nodes[n].first_child + t;
        aabb = aabb.build_quadrant(t, mid);
    }
    data[n] = {mass, pos};
    data[o] = data[idx];
    data[idx].mass.val += mass;
    auto r = mass / data[idx].mass;
    data[idx].centre = data[idx].centre * (1 - r) + pos * r;
    for (size_t i = 1; i < update.size(); ++i) {
        data[update[i]] = data[idx];
    }
    update.clear();
}

void gravity::BH_QuadTree::clear() {
    nodes.clear();
    data.clear();
    nodes.emplace_back();
    data.emplace_back();
}
