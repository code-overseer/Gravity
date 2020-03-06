#include "../include/World.hpp"
#include "../include/components.hpp"
#include <vector>
#include <unordered_map>
#include <exception>

void gravity::World::update() {
    // update forces
    // check for collisions
    // move
    // update gravity grid
    // update collider grid

}

void gravity::World::preDraw(Renderer & renderer) const {
    // update camera
    // update local to worlds components
    // cull w/ Circle collider
    // update ltw buffer
    renderer.updateCamera(_mainCamera);
    auto& ltw = renderer.localToWorlds_[renderer._buffer_idx];
    auto v = _registry.view<const components::LocalToWorld>();
    if (ltw.isEmpty()) {
        ltw = Renderer::mallocBuffer(renderer._device, v.raw(), v.size()*sizeof(components::LocalToWorld), mtl_cpp::Managed);
    } else {
        ltw.copy(v.raw(), v.size()*sizeof(components::LocalToWorld));
    }
    renderer._instanceCount = v.size();
    static bool s = true;
    if (s) {
        auto m = mathsimd::matmul(v.raw()->val, mathsimd::float4(renderer._vertices.raw<mathsimd::float2>()[1],0,1));
        std::cout<<"world: "<<m<<std::endl;
        auto vi = renderer._camera[renderer._buffer_idx].raw<mathsimd::float4x4>()[0];
        m = mathsimd::matmul(vi, m);
        std::cout<<"cam: "<<m<<std::endl;
        auto pr = renderer._camera[renderer._buffer_idx].raw<mathsimd::float4x4>()[1];
        m = mathsimd::matmul(pr, m);
        std::cout<<"screen: "<<m<<std::endl;
        std::cout<<v.size()<<std::endl;
        s = false;
    }
}

int gravity::World::_gravityCellIndex(mathsimd::float2 p) {
    using namespace components;
    using namespace mathsimd;
    p = p - _bounds.min;
    p = p / (_bounds.max - _bounds.min);
    p = p * float2{static_cast<float>(4), static_cast<float>(2)};
    p = float2(std::floor(p.x()), std::floor(p.y()));
    return static_cast<int>(dot(p, {1, static_cast<float>(2)}));
}

void gravity::World::initializeParticles() {
    using namespace components;
    using namespace mathsimd;
    std::vector<entt::entity> entities(10000);
    _registry.create(entities.begin(), entities.end());

    auto const start = _bounds.min + float2(200,200);
    int i = 0;
    for (auto &e : entities) {
        int dx = i % 100;
        int dy = i / 100;
        auto pos = start + 16.f * float2(static_cast<float>(dx),static_cast<float>(dy));
        switch (_gravityCellIndex(pos)) {
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
        float r = _rand.rnd(1.f,15.f);
        _registry.assign<Position>(e, pos);
        _registry.assign<Velocity>(e, 0,0);
        _registry.assign<CircleCollider>(e, r);
        _registry.assign<Mass>(e, _rand.rnd(1.f,100.f));
        _registry.assign<Restitution>(e, _rand.rnd(0.5f,1.f));
        _registry.assign<LocalToWorld>(e, LocalToWorld::fromPositionAndRadius(pos, r));
        ++i;
    }

}
