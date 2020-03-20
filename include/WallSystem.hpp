#ifndef GRAVITY_WALLSYSTEM_HPP
#define GRAVITY_WALLSYSTEM_HPP

#include <entt/entt.hpp>
#include "System.hpp"

namespace gravity::systems {

    class WallSystem : public System {
        friend class gravity::World;
        WallSystem(gravity::World& w) : System(w) {};
    public:
        void update(float delta) override;
    };
}


#endif //GRAVITY_WALLSYSTEM_HPP
