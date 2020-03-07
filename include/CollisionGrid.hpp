#ifndef GRAVITY_COLLISIONGRID_HPP
#define GRAVITY_COLLISIONGRID_HPP

#include <mathsimd.hpp>
#include <entt/entt.hpp>
#include "AABB.hpp"
#include "components.hpp"
#include <utility>

namespace gravity {
    struct CollisionGrid {
        struct LooseCell {
            int head = -1;
            AABB box{0,0,0,0};
        };
        struct LooseNode {
            int cell = -1;
            int next = -1;
        };
        struct TightCell {
            int head = -1;
            TightCell() = default;
        };
        struct ElementNode {
            entt::entity val;
            int next = -1;
            explicit ElementNode(entt::entity const& entity) : val(entity) {}
        };
        int const _width;
        int const _height;
        AABB const _world;
        std::vector<ElementNode> _entities;
        std::vector<LooseCell> _looseCells;
        std::vector<LooseNode> _looseNodes;
        std::vector<TightCell> _tightCells;
        CollisionGrid(AABB world, int width, int height, int particles);

        void add(entt::entity const &e, mathsimd::float2 const &pos, components::CircleCollider const&radius);


    };


}

#endif //GRAVITY_COLLISIONGRID_HPP
