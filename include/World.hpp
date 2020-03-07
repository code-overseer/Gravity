

#ifndef GRAVITY_WORLD_HPP
#define GRAVITY_WORLD_HPP

#include <taskflow.hpp>
#include <entt/entt.hpp>
#include <mathsimd.hpp>
#include "AABB.hpp"
#include "Renderer.hpp"
#include "CollisionGrid.hpp"

namespace gravity {
    struct World {
    private:
        entt::registry _registry;
        tf::Executor _jobManager;
        mathsimd::Random _rand{1234};
        const AABB _bounds{-1000,-1000,1000,1000};
        Camera _mainCamera;
        CollisionGrid _collisionGrid;
    public:
        void initializeParticles();
        World() = default;
        void update();
        void preDraw(Renderer& renderer) const;
    };
}

#endif //GRAVITY_WORLD_HPP
