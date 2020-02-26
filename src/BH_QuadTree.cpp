

#include "../include/BH_QuadTree.hpp"

void gravity::BH_QuadTree::insert(const mathsimd::float2& pos, float mass)  {

    int idx = 0;
    bool will_loop = true;
    AABB current = Root_box;
    while (will_loop) {
        auto old_data = data[idx];
        data[idx].mass += mass;
        auto tmp = mass / data[idx].mass;
        data[idx].centre = (data[idx].centre * (1 - tmp)) + (tmp * pos);
        switch (data[idx].type) {
            case Empty: {
                data[idx].type = External;
                will_loop = false;
                break;
            }
            case External: {
                data[idx].type = Internal;
                nodes[idx].first_child = nodes.size();
                auto d = data[idx].depth + 1;
                for (int i = 0; i < 4; ++i) nodes.emplace_back();
                for (int i = 0; i < 4; ++i) data.emplace_back(d);
                auto o = current.quadrant_index(old_data.centre);
                auto n = current.quadrant_index(pos);
                if (o.first == n.first) {
                    data[n.first] = old_data;
                    data[n.first].type = External;
                    current = n.second;
                    idx = nodes[idx].first_child + n.first;
                    break;
                }
                data[n.first].mass = mass;
                data[n.first].centre = pos;
                data[n.first].type = External;
                data[o.first].mass = old_data.mass;
                data[o.first].centre = old_data.centre;
                data[o.first].type = External;
                will_loop = false;
                break;
            }
            case Internal: {
                auto n = current.quadrant_index(pos);
                current = n.second;
                idx = nodes[idx].first_child + n.first;
                break;
            }
        }

    }

}

void gravity::BH_QuadTree::clear() {
    nodes.clear();
    data.clear();
}
