

#ifndef GRAVITY_MOVEMENTSYSTEM_HPP
#define GRAVITY_MOVEMENTSYSTEM_HPP

#include <entt/entt.hpp>
#include "System.hpp"
namespace gravity::systems {
    class MovementSystem : System {
    public:
        explicit MovementSystem(entt::registry *reg) : System(reg) {}
        void update(float delta) override;
    };
}



#endif //GRAVITY_MOVEMENTSYSTEM_HPP
