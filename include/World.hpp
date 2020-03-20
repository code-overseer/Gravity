

#ifndef GRAVITY_WORLD_HPP
#define GRAVITY_WORLD_HPP

#include <taskflow.hpp>
#include <entt/entt.hpp>
#include <mathsimd.hpp>
#include "AABB.hpp"
#include "Renderer.hpp"

namespace gravity::systems {
    class System;
}

namespace gravity {

    struct World {
    private:
        World();
        static World& _instance() {
            static World w;
            return w;
        }
        entt::registry _registry;
        tf::Executor _executor;
        mathsimd::Random _rand{1234};
        const AABB _bounds{-2000,-2000,2000,2000};
        Camera _mainCamera;
        std::vector<std::unique_ptr<gravity::systems::System>> _systems;
    public:
        World(World const&) = delete;
        ~World();
        void update();
        void preDraw(Renderer& renderer);
        static void Update() { _instance().update(); }
        static void PreDraw(Renderer& renderer) { _instance().preDraw(renderer); }
        tf::Executor& executor() { return _executor; }
        entt::registry& registry() { return _registry; }
        AABB const& bounds() { return _bounds; }
    };
}

#endif //GRAVITY_WORLD_HPP
