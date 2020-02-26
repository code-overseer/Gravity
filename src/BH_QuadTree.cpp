

#include "../include/BH_QuadTree.hpp"

void gravity::BH_QuadTree::insert(const mathsimd::float2& pos, float mass)  {

    int idx = 0;
    AABB current = Root_box;
    while (idx >= 0) {
        bool was_empty = (data[idx].mass == 0.f);
        data[idx].mass += mass;
        auto tmp = mass / data[idx].mass;
        data[idx].centre = (data[idx].centre * (1 - tmp)) + (tmp * pos);
        if (was_empty) return;
        // if no children
        if (nodes[idx].child < 0) {
            nodes[idx].child = nodes.size();
            for (int i = 0; i < 4; ++i) nodes.emplace_back();
            for (int i = 0; i < 4; ++i) data.emplace_back();
        }

        mathsimd::float2 mid;
        switch (current.quadrant_mask(pos, mid)) {
            case 0xff:
                idx = nodes[idx].child + 2;
                current.max = mid;
                break;
            case 0x00:
                idx = nodes[idx].child + 1;
                current.min = mid;
                break;
            case 0xf0:
                idx = nodes[idx].child;
                current.min.y() = mid.y();
                current.max.x() = mid.x();
                break;
            case 0x0f:
                idx = nodes[idx].child + 3;
                current.min.x() = mid.x();
                current.max.y() = mid.y();
                break;
        }
    }


}

void gravity::BH_QuadTree::clear() {

}
