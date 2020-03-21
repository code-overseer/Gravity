

#ifndef GRAVITY_COLLISIONSYSTEM_HPP
#define GRAVITY_COLLISIONSYSTEM_HPP

#include <entt/entt.hpp>
#include <vector>
#include <utility>
#include "System.hpp"
#include "CollisionGrid.hpp"

namespace gravity::systems{

    class CollisionSystem : public System {
    private:
        CollisionGrid _grid;
        void _prepareGrid();
        std::vector<std::pair<entt::entity,entt::entity>> _collisions;
        void _findCollisions();

        friend class gravity::World;
        CollisionSystem(gravity::World& w, int grid_width, int grid_height, int particles);
    public:
        void update(float delta) override;
    };
}


#endif //GRAVITY_COLLISIONSYSTEM_HPP
