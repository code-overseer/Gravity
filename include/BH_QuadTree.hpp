#ifndef GRAVITY_BH_QUADTREE_HPP
#define GRAVITY_BH_QUADTREE_HPP

#include <mathsimd.hpp>
#include <utility>
#include <vector>
#include "AABB.hpp"

namespace gravity {

    struct BH_QuadTree {
        struct BH_Node {
            union Mass {
                float val = 0;
                unsigned int bits;
                Mass(float const &f) : val(f) {}
                Mass() = default;
                operator float() const { return val; }
                Mass& operator+=(float const &f) { val += f; return *this; }
                Mass& operator-=(float const &f) { val -= f; return *this; }
                Mass& operator*=(float const &f) { val *= f; return *this; }
                Mass& operator/=(float const &f) { val /= f; return *this; }
            };
            mathsimd::float2 centre{0,0};
            mathsimd::float2 mid{0,0};
            AABB box;
            Mass mass;
            int first_child = INT32_MIN;
            explicit BH_Node(AABB box) : box(std::move(box)) {};
            BH_Node(AABB box, float mass, mathsimd::float2 const &pos) : box(std::move(box)), mass(mass), centre(pos) {};
            BH_Node(BH_Node const &other) = default;
            BH_Node(BH_Node &&other) = default;
            [[nodiscard]] inline bool isEmpty() const { return !(0u ^ mass.bits); }
        };

        std::vector<BH_Node> nodes;
        float const SqrTheta;
        BH_QuadTree(BH_QuadTree const& other) = delete;
        BH_QuadTree(BH_QuadTree&& other) noexcept : SqrTheta(other.SqrTheta), nodes(std::move(other.nodes)) {}
        BH_QuadTree(float t_sqr, AABB aabb, unsigned int count) : SqrTheta(t_sqr) {
            nodes.reserve(count);
            nodes.emplace_back(std::move(aabb));
        }

        void insert(const mathsimd::float2& pos, float mass);

        void clear();


    };
}



#endif //GRAVITY_BH_QUADTREE_HPP
