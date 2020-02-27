#ifndef GRAVITY_BH_QUADTREE_HPP
#define GRAVITY_BH_QUADTREE_HPP

#include <mathsimd.hpp>
#include <utility>
#include <vector>
#include "AABB.hpp"

namespace gravity {

    struct BH_QuadTree {
        enum NodeType : unsigned short { Empty, External, Internal };
        struct BH_Node {
            struct Data {
                mathsimd::float2 centre{0,0};
                float mass = 0;
                Data() = default;
                Data(float mass, mathsimd::float2 const &pos) : mass(mass), centre(pos) {};
            };
            int first_child = INT32_MIN;
            NodeType type = Empty;
            unsigned short depth;
            explicit BH_Node(unsigned short d) : depth(d) {}
        };

        std::vector<BH_Node> nodes;
        std::vector<BH_Node::Data> data;
        float const SqrTheta;
        AABB const Root_box;
        BH_QuadTree(BH_QuadTree const& other) = delete;
        BH_QuadTree(BH_QuadTree&& other) noexcept : SqrTheta(other.SqrTheta), Root_box(other.Root_box), nodes(std::move(other.nodes)) {}
        BH_QuadTree(float t_sqr, AABB aabb, unsigned int count) : SqrTheta(t_sqr), Root_box(std::move(aabb)) {
            count += ((count & (count - 1)) == 0);
            nodes.reserve(count);
            data.reserve(count);
            nodes.emplace_back(0);
            data.emplace_back();
        }

        void insert(const mathsimd::float2& pos, float mass);

        void clear();


    };
}



#endif //GRAVITY_BH_QUADTREE_HPP
