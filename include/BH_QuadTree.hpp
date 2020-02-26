#ifndef GRAVITY_BH_QUADTREE_HPP
#define GRAVITY_BH_QUADTREE_HPP

#include <mathsimd.hpp>
#include <utility>
#include <vector>
#include "AABB.hpp"

namespace gravity {
    struct BH_Node {
        struct alignas(16) Data {
            mathsimd::float2 centre{0,0};
            float mass = 0;
            Data() = default;
            Data(mathsimd::float2 const &c, float const &m) : centre{c}, mass(m) {}
        };
        int child = INT32_MIN;
        BH_Node() = default;
    };
    // Possible improvement: split data and node

    struct BH_QuadTree {
        std::vector<BH_Node> nodes;
        std::vector<BH_Node::Data> data;
        float const SqrTheta;
        AABB const Root_box;
        BH_QuadTree(BH_QuadTree const& other) = delete;
        BH_QuadTree(BH_QuadTree&& other) noexcept : SqrTheta(other.SqrTheta), Root_box(other.Root_box), nodes(std::move(other.nodes)) {}
        BH_QuadTree(float t_sqr, AABB aabb, unsigned int count) : SqrTheta(t_sqr), Root_box(std::move(aabb)) {
            if ((count & (count - 1)) == 0) count += 2;
            nodes.reserve(count);
            data.reserve(count);
            nodes.emplace_back();
            data.emplace_back();
        }

        void insert(const mathsimd::float2& pos, float mass);

        void clear();


    };
}



#endif //GRAVITY_BH_QUADTREE_HPP
