

#ifndef GRAVITY_COLLISIONSYSTEM_HPP
#define GRAVITY_COLLISIONSYSTEM_HPP

#include <entt/entt.hpp>
#include "System.hpp"
#include "CollisionGrid.hpp"

namespace gravity::systems{

    class CollisionSystem : System {
    public:
        explicit CollisionSystem(entt::registry *reg) : System(reg) {}
        void update(float delta) override;
    private:

    };
}


#endif //GRAVITY_COLLISIONSYSTEM_HPP
