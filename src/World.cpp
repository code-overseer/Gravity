#include "../include/World.hpp"
#include "../include/components.hpp"
#include "../include/Utils.hpp"
#include "../include/CollisionSystem.hpp"
#include "../include/MovementSystem.hpp"
#include "../include/BoundingSystem.hpp"
#include <vector>
#include <unordered_map>
#include <chrono>

void gravity::World::update() {
    using namespace std::chrono;
    static auto timer = high_resolution_clock::now();
    auto deltaTime = duration<double>(high_resolution_clock::now() - timer).count();
    timer = high_resolution_clock::now();

    _movement->update(deltaTime);
    _collision->update(deltaTime);
    _bounding->update(deltaTime);
}

void gravity::World::preDraw(Renderer & renderer) {
    // update camera
    renderer.updateCamera(_mainCamera);
    // update local to worlds components
    // cull w/ Circle collider
    _registry.view<components::Position, components::CircleCollider, components::LocalToWorld>().each([](components::Position const&pos,
                                                                                                         components::CircleCollider const& col, components::LocalToWorld& ltw) {
        ltw = components::LocalToWorld::fromPositionAndRadius(pos.val, col.radius);
    });
    // update ltw buffer
    auto& ltw = renderer.localToWorlds_[renderer._buffer_idx];
    auto v = _registry.view<components::LocalToWorld>();
    if (ltw.isEmpty()) {
        ltw = Renderer::mallocBuffer(renderer._device, v.raw(), v.size()*sizeof(components::LocalToWorld), mtl_cpp::Managed);
    } else {
        ltw.copy(v.raw(), v.size()*sizeof(components::LocalToWorld));
    }
    renderer._instanceCount = v.size();
}

gravity::World::World() {
    using namespace components;
    using namespace systems;
    using namespace mathsimd;
    int n = 30;
    std::vector<entt::entity> entities(n * n);
    _registry.create(entities.begin(), entities.end());

    auto const start = _bounds.min + float2(200,200);
    int i = 0;
    for (auto &e : entities) {
        int dx = i % n;
        int dy = i / n;
        auto pos = start + 3600.f/static_cast<float>(n) * float2(static_cast<float>(dx),static_cast<float>(dy));
        switch (getIndex(pos, _bounds, 4, 2)) {
            case Grid0:
                _registry.assign<entt::tag<Grid0>>(e);
                break;
            case Grid1:
                _registry.assign<entt::tag<Grid1>>(e);
                break;
            case Grid2:
                _registry.assign<entt::tag<Grid2>>(e);
                break;
            case Grid3:
                _registry.assign<entt::tag<Grid3>>(e);
                break;
            case Grid4:
                _registry.assign<entt::tag<Grid4>>(e);
                break;
            case Grid5:
                _registry.assign<entt::tag<Grid5>>(e);
                break;
            case Grid6:
                _registry.assign<entt::tag<Grid6>>(e);
                break;
            case Grid7:
                _registry.assign<entt::tag<Grid7>>(e);
                break;
            default:
                throw std::invalid_argument("Expected range 0 - 7");
        }
        float r = _rand.rnd(10.f,20.f);
        _registry.assign<Position>(e, pos);
        _registry.assign<Velocity>(e, _rand.rnd(-124.f,124.f),_rand.rnd(-124.f,124.f));
        _registry.assign<Acceleration>(e, mathsimd::float2::zero());
        _registry.assign<CircleCollider>(e, r);
        _registry.assign<Mass>(e, _rand.rnd(10.f,80.f));
        _registry.assign<Restitution>(e, _rand.rnd(0.75f,1.f));
        _registry.assign<Checked>(e);
        _registry.assign<LocalToWorld>(e, LocalToWorld::fromPositionAndRadius(pos, r));
        ++i;
    }

    _movement = new MovementSystem(&_registry);
    _collision = new CollisionSystem(&_registry, _bounds, 80, 80, entities.size());
    _bounding = new BoundingSystem(&_registry, _bounds);

}

gravity::World::~World()  {
    delete _collision;
    delete _movement;
    delete _bounding;
    _collision = nullptr;
    _movement = nullptr;
    _bounding = nullptr;
}
