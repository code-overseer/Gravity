#include "../include/World.hpp"
#include "../include/components.hpp"
#include "../include/CollisionSystem.hpp"

gravity::systems::CollisionSystem::CollisionSystem(gravity::World &w, int grid_width, int grid_height,
                                                   int particles)  : System(w),
                                                                     _grid(w.bounds(), grid_width, grid_height, particles) {}

void gravity::systems::CollisionSystem::update(float delta) {
    _prepareGrid();
    _findCollisions();
    _collide();
}

void gravity::systems::CollisionSystem::_prepareGrid() {
    using namespace gravity::components;
    auto view = world().registry().view<Position, CircleCollider>();
    view.each([this](entt::entity const& e, Position const& p, CircleCollider const& r){
        _grid.add(e, p.val, r);
    });
    _grid.flushLooseCells();
}

void gravity::systems::CollisionSystem::_findCollisions() {
    using namespace gravity::components;
    _collisions.clear();
    auto view = world().registry().view<Position, CircleCollider, Checked, Velocity>();

    for (auto entity : view) {
        auto& stat = view.get<Checked>(entity);
        stat.val = true;
        auto this_pos = view.get<Position>(entity);
        auto this_col = view.get<CircleCollider>(entity);
        auto const &result = _grid.query(this_col.toAABB(this_pos.val));

        for (auto const& other : result) {
            if (other == entity || view.get<Checked>(other).val) continue;
            auto pos = view.get<Position>(other);
            auto col = view.get<CircleCollider>(other);
            auto R_sqr = (this_col.radius + col.radius)*1.005f;
            R_sqr *= R_sqr;
            auto nba = this_pos.val - pos.val;
            auto sqrMag = nba.sqrMagnitude();
            auto va = view.get<Velocity>(entity).val;
            auto vb = view.get<Velocity>(other).val;
            if (sqrMag < R_sqr && dot(va - vb, nba) < 0) {
                _collisions.emplace_back(entity, other);
            }
        }
    }
    _grid.clear();
    world().registry().view<Checked>().each([](Checked &c) { c.val = false; });
}

void gravity::systems::CollisionSystem::_collide() {
    using namespace gravity::components;
    auto view = world().registry().view<Velocity, Mass, Restitution, Position>();
    for (auto const& collision : _collisions) {
        auto cr = 0.5f * (view.get<Restitution>(collision.first).val + view.get<Restitution>(collision.second).val);
        auto dir = (view.get<Position>(collision.first).val - view.get<Position>(collision.second).val).normalized();
        auto& vel_a = view.get<Velocity>(collision.first);
        auto& vel_b = view.get<Velocity>(collision.second);
        auto ua = dot(dir, vel_a.val) * dir;
        auto ub = dot(dir, vel_b.val) * dir;

        auto ma = view.get<Mass>(collision.first).val;
        auto mb = view.get<Mass>(collision.second).val;
        auto inv_sum = 1.0 / (ma + mb);
        auto va = inv_sum * (cr * mb *(ub - ua) + ma * ua + mb * ub);
        auto vb = inv_sum * (cr * ma * (ua - ub) + ma * ua + mb * ub);
        vel_a.val = vel_a.val - ua + va;
        vel_b.val = vel_b.val - ub + vb;
    }
}
