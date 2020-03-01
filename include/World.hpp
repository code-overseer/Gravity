

#ifndef GRAVITY_WORLD_HPP
#define GRAVITY_WORLD_HPP
#include "entt/entt.hpp"
#include "mathsimd.hpp"
#include "AABB.hpp"

namespace gravity {
    struct World {
    private:
        entt::registry _registry;
        mathsimd::Random _rand{1234};
        const AABB _bounds{-1000,-1000,1000,1000};
        void _initializeParticles();
        int _gravityCellIndex(mathsimd::float2 p);

    public:
        World() = default;
        void update();
    };
}

#endif //GRAVITY_WORLD_HPP
