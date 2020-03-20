

#include "../include/BH_QuadTree.hpp"

void gravity::BH_QuadTree::insert(const mathsimd::float2& pos, float mass)  {

    int idx = 0;
    while (nodes[idx].first_child > 0) {
        nodes[idx].mass += mass;
        auto tmp = mass / nodes[idx].mass;
        nodes[idx].centre = (nodes[idx].centre * (1 - tmp)) + (tmp * pos);
        idx = nodes[idx].first_child + nodes[idx].box.quadrant_idx(pos, nodes[idx].mid);
    }
    if (!nodes[idx].isEmpty()) {
        nodes[idx].first_child = nodes.size();
        nodes[idx].mid = nodes[idx].centre + 0.5f * (pos - nodes[idx].centre);
        for (int i = 0; i < 4; ++i) nodes.emplace_back(nodes[idx].box.build_quadrant(i, nodes[idx].mid));
        auto n = nodes[idx].box.quadrant_idx(pos, nodes[idx].mid) + nodes[idx].first_child;
        auto o = nodes[idx].box.quadrant_idx(nodes[idx].centre, nodes[idx].mid) + nodes[idx].first_child;
        nodes[n].mass.val = mass;
        nodes[n].centre = pos;
        nodes[o].mass.val = nodes[idx].mass.val;
        nodes[o].centre = nodes[idx].centre;
    }

    nodes[idx].mass.val += mass;
    auto r = mass / nodes[idx].mass;
    nodes[idx].centre = nodes[idx].centre * (1 - r) + pos * r;

}

void gravity::BH_QuadTree::clear() {
    auto b = nodes[0].box;
    nodes.clear();
    nodes.emplace_back(b);
}

mathsimd::float2 gravity::BH_QuadTree::traverse(mathsimd::float2 pos) {
    using namespace mathsimd;
    float2 acc = float2::zero();
    _traversal.clear();
    _traversal.emplace_back(0);
    while (!_traversal.empty()) {
        auto i = _traversal.back();
        _traversal.pop_back();
        if (nodes[i].isEmpty()) continue;
        auto dir = nodes[i].centre - pos;
        auto d_sqr = dir.sqrMagnitude();
        if (d_sqr < mathsimd::EPSILON_F*mathsimd::EPSILON_F) continue;
        float inv_d_sqr = 1.f / d_sqr;
        float theta_sqr = (nodes[i].box.max - nodes[i].box.min).sqrMagnitude() * inv_d_sqr;

        if (nodes[i].isExternal() || theta_sqr < SqrTheta) {
             acc = acc + (nodes[i].mass * inv_d_sqr) * dir.normalized();
        } else {
            for (int j = 3; j >= 0; --j) {
                _traversal.emplace_back(nodes[i].first_child + j);
            }
        }
    }
    return acc;
}
