

#ifndef GRAVITY_WORLD_HPP
#define GRAVITY_WORLD_HPP
#include "entt/entt.hpp"
#include "mathsimd.hpp"
#include "AABB.hpp"
#include "Renderer.hpp"

namespace gravity {
    struct World {
    private:
        entt::registry _registry;
        mathsimd::Random _rand{1234};
        const AABB _bounds{-1000,-1000,1000,1000};
        int _gravityCellIndex(mathsimd::float2 p);
        Camera _mainCamera;
    public:
        void initializeParticles();
        World() = default;
        void update();
        void preDraw(Renderer& renderer) const;
    };
}

#endif //GRAVITY_WORLD_HPP
