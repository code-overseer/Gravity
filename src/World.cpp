#include "../include/World.hpp"
#include "../include/components.hpp"
#include "../include/Utils.hpp"
#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>

void gravity::World::update() {
    using namespace std::chrono;
    static auto timer = high_resolution_clock::now();
    auto deltaTime = duration<double>(high_resolution_clock::now() - timer).count();
    timer = high_resolution_clock::now();

    // update forces
    auto pv_view = _registry.view<components::Position,components::Velocity>();
    pv_view.each([=](components::Position &p, components::Velocity const& v) {
        p.val = p.val + v.val * deltaTime;
    });

    // check for collisions
    auto pc_view = _registry.view<components::Position, components::CircleCollider>();
    pc_view.each([this](entt::entity const& e,components::Position const& p,components::CircleCollider const& r){
        _collisionGrid.add(e,p.val,r);
    });
    _collisionGrid.flushLooseCells();

    std::vector<std::pair<entt::entity, entt::entity>> collisions;
    auto view = _registry.view<components::Position, components::CircleCollider, components::Checked, components::Velocity>();
    for (auto entity : view) {
        auto& stat = view.get<components::Checked>(entity);
        stat.val = true;
        auto this_pos = view.get<components::Position>(entity);
        auto this_col = view.get<components::CircleCollider>(entity);
        auto const &result = _collisionGrid.query(this_col.toAABB(this_pos.val));
        for (auto const& other : result) {
            if (other == entity || view.get<components::Checked>(other).val) continue;
            auto& pos = view.get<components::Position>(other);
            auto col = view.get<components::CircleCollider>(other);
            auto R_sqr = std::pow((this_col.radius + col.radius)*1.05f,2);
            auto nba = this_pos.val - pos.val;
            auto sqrMag = nba.sqrMagnitude();
            auto va = view.get<components::Velocity>(entity).val;
            auto vb = view.get<components::Velocity>(other).val;
            auto denom = dot(va - vb, nba);
            if (sqrMag < R_sqr && denom < 0) {
                collisions.emplace_back(entity, other);
                break;
            }
        }
    }

    _collisionGrid.clear();

    auto col_view = _registry.view<components::Velocity, components::Mass, components::Restitution, components::Position>();
    for (auto const& collision : collisions) {
        auto cr = col_view.get<components::Restitution>(collision.first).val + col_view.get<components::Restitution>(collision.second).val;
        cr *= 0.5f;
        auto ma = col_view.get<components::Mass>(collision.first).val;
        auto mb = col_view.get<components::Mass>(collision.second).val;

        auto dir = col_view.get<components::Position>(collision.first).val - col_view.get<components::Position>(collision.second).val;
        dir = dir.normalized();
        auto& vel_a = col_view.get<components::Velocity>(collision.first);
        auto& vel_b = col_view.get<components::Velocity>(collision.second);
        auto ua = dot(dir, vel_a.val) * dir;
        auto ub = dot(dir, vel_b.val) * dir;

        auto inv_sum = 1.0 / (ma + mb);
        auto va = inv_sum * (cr * mb *(ub - ua) + ma * ua + mb * ub);
        auto vb = inv_sum * (cr * ma * (ua - ub) + ma * ua + mb * ub);
        vel_a.val = vel_a.val - ua + va;
        vel_b.val = vel_b.val - ub + vb;
    }

    auto b = _bounds;
    auto pvc_view = _registry.view<components::Position,components::Velocity, components::CircleCollider>();
    pvc_view.each([b](components::Position const &p, components::Velocity& v, components::CircleCollider const& c) {
        auto tr = p.val + c.radius;
        auto bl = p.val - c.radius;
        bool r = tr.x() > b.max.x(), l = bl.x() < b.min.x(), t = tr.y() > b.max.y(), btm = bl.y() < b.min.y();
        auto tmp = mathsimd::float2(((r && v.val.x() > 0) || (l && v.val.x() < 0)),
                ((t && v.val.y() > 0) || (btm && v.val.y() < 0)));
        v.val = v.val - 2 * tmp * v.val;
    });

    _registry.view<components::Checked>().each([](components::Checked &c) { c.val = false; });


    // update gravity grid
    // update collider grid

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
    using namespace mathsimd;
    int n = 4;
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

    _collisionGrid = CollisionGrid(_bounds, 80, 80, entities.size());

}
