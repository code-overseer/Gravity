

#ifndef GRAVITY_BOUNDINGSYSTEM_HPP
#define GRAVITY_BOUNDINGSYSTEM_HPP
#include "System.hpp"
#include "AABB.hpp"
#include <entt/entt.hpp>
namespace gravity::systems {

    class BoundingSystem : public System {
    private:
        AABB _world;
    public:
        BoundingSystem(entt::registry *reg, AABB const&world) : System(reg), _world(world) {};
        void update(float delta) override;
    };
}


#endif //GRAVITY_BOUNDINGSYSTEM_HPP
