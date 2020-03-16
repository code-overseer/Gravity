

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
        const AABB _bounds{-2000,-2000,2000,2000};
        Camera _mainCamera;
        CollisionGrid _collisionGrid;
    public:
        World();
        void update();
        void preDraw(Renderer& renderer);
    };
}

#endif //GRAVITY_WORLD_HPP
