

#ifndef GRAVITY_WORLD_HPP
#define GRAVITY_WORLD_HPP

#include <taskflow.hpp>
#include <entt/entt.hpp>
#include <mathsimd.hpp>
#include "AABB.hpp"
#include "Renderer.hpp"
#include "System.hpp"

namespace gravity {
    struct World {
    private:
        entt::registry _registry;
        tf::Executor _jobManager;
        mathsimd::Random _rand{1234};
        const AABB _bounds{-2000,-2000,2000,2000};
        Camera _mainCamera;
        systems::System* _collision = nullptr;
        systems::System* _movement = nullptr;
        systems::System* _bounding = nullptr;
    public:
        World();
        ~World();
        void update();
        void preDraw(Renderer& renderer);
    };
}

#endif //GRAVITY_WORLD_HPP
