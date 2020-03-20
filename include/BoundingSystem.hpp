#ifndef GRAVITY_BOUNDINGSYSTEM_HPP
#define GRAVITY_BOUNDINGSYSTEM_HPP

#include <entt/entt.hpp>
#include "System.hpp"

namespace gravity::systems {

    class BoundingSystem : public System {
        friend class gravity::World;
        BoundingSystem(gravity::World& w) : System(w) {};
    public:
        void update(float delta) override;
    };
}


#endif //GRAVITY_BOUNDINGSYSTEM_HPP
