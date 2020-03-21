

#ifndef GRAVITY_WORLD_HPP
#define GRAVITY_WORLD_HPP

#include <taskflow.hpp>
#include <entt/entt.hpp>
#include <mathsimd.hpp>
#include <memory>
#include <chrono>
#include "AABB.hpp"
#include "RenderSystem.hpp"

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
        const AABB _bounds;
        Camera _mainCamera;
        decltype(std::chrono::high_resolution_clock::now()) _updateTimer;
        decltype(std::chrono::high_resolution_clock::now()) _drawTimer;
        bool _interruptFlag = false;
        std::vector<std::unique_ptr<gravity::systems::System>> _systems;
        systems::RenderSystem* _renderer = nullptr;
    public:
        World(World const&) = delete;
        ~World();
        static World& Default() { return _instance(); }
        void interrupt() { _interruptFlag = true; }
        void update();
        void draw();
        static void Update() { _instance().update(); }
        static void PreDraw() { _instance().draw(); }
        tf::Executor& executor() { return _executor; }
        entt::registry& registry() { return _registry; }
        AABB const& bounds() { return _bounds; }
        Camera& mainCamera() { return _mainCamera; }
    };
}

#endif //GRAVITY_WORLD_HPP
