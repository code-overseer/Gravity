

#ifndef GRAVITY_COLLISIONSYSTEM_HPP
#define GRAVITY_COLLISIONSYSTEM_HPP

#include <entt/entt.hpp>
#include "System.hpp"
#include "CollisionGrid.hpp"
#include <vector>
#include <utility>

namespace gravity::systems{

    class CollisionSystem : public System {
    private:
        CollisionGrid _grid;
        void _prepareGrid();
        std::vector<std::pair<entt::entity,entt::entity>> _collisions;
        void _findCollisions();
        void _collide();
    public:
        CollisionSystem(entt::registry *reg, AABB const&world, int grid_width, int grid_height, int particles) : System(reg),
        _grid(world, grid_width, grid_height, particles) {}
        CollisionSystem(CollisionSystem&& system) noexcept : System(std::move(system)),
        _collisions(std::move(system._collisions)), _grid(std::move(system._grid)) {};
        void update(float delta) override;
    };
}


#endif //GRAVITY_COLLISIONSYSTEM_HPP
