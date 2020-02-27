

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
                    idx = nodes[idx].first_child + n.first;
                    data[idx].mass = old_data.mass;
                    data[idx].centre = old_data.centre;
                    data[idx].type = External;
                    current = n.second;
                    break;
                }
                auto nid = nodes[idx].first_child + n.first;
                auto oid = nodes[idx].first_child + o.first;
                data[nid].mass = mass;
                data[nid].centre = pos;
                data[nid].type = External;
                data[oid].mass = old_data.mass;
                data[oid].centre = old_data.centre;
                data[oid].type = External;
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
