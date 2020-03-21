#include "../include/World.hpp"
#include "../include/components.hpp"
#include "../include/CollisionSystem.hpp"

gravity::systems::CollisionSystem::CollisionSystem(gravity::World &w, int grid_width, int grid_height, int particles) : System(w),
                                                                     _grid(w.bounds(), grid_width, grid_height, particles) {}

void gravity::systems::CollisionSystem::update(float delta) {
    _prepareGrid();
    _findCollisions();

    using namespace gravity::components;
    auto& reg = world().registry();
    auto v_bh = reg.view<BlackHole>();
    auto view = reg.view<Mass, Position, Velocity, CircleCollider>();
    for (auto const& bh : v_bh) {
        if (!reg.valid(bh)) continue;
        auto pos = view.get<Position>(bh);
        auto r = view.get<CircleCollider>(bh);
        auto &result = _grid.query(r.toAABB(pos.val));
        auto& mass = view.get<Mass>(bh);
        auto& vel = view.get<Velocity>(bh);
        auto m = mass.val;
        auto p = m * vel.val;

        for (auto & entity_b : result) {
            if (!reg.valid(entity_b)) continue;
            auto R_b = view.get<CircleCollider>(entity_b);
            auto R_sqr = (r.radius + R_b.radius) * 1.005f;
            auto pos_b = view.get<Position>(entity_b);
            R_sqr *= R_sqr;
            if (entity_b == bh || (pos.val - pos_b.val).sqrMagnitude() > R_sqr) continue;
            auto mb = view.get<Mass>(entity_b).val;
            m += mb;
            p = p + (mb * view.get<Velocity>(entity_b).val);
            reg.destroy(entity_b);
        }
        mass.val = m;
        vel.val = p / m;

    }
}

void gravity::systems::CollisionSystem::_prepareGrid() {
    using namespace gravity::components;
    _grid.clear();
    auto view = world().registry().view<Position, CircleCollider>();
    view.each([this](entt::entity const& e, Position const& p, CircleCollider const& r){
        _grid.add(e, p.val, r);
    });
    _grid.flushLooseCells();
}

void gravity::systems::CollisionSystem::_findCollisions() {
    using namespace gravity::components;
    auto view = world().registry().view<Position, Mass, CircleCollider, Velocity, Acceleration, Restitution>();
    for (auto const &entity_a : view) {
        auto pos_a = view.get<Position>(entity_a);
        auto R_a = view.get<CircleCollider>(entity_a);
        auto const &result = _grid.query(R_a.toAABB(pos_a.val));

        for (auto const& entity_b : result) {
            if (entity_b == entity_a) continue;
            auto pos_b = view.get<Position>(entity_b);
            auto R_b = view.get<CircleCollider>(entity_b);

            auto R_sqr = (R_a.radius + R_b.radius) * 1.005f;
            R_sqr *= R_sqr;

            auto dba = pos_a.val - pos_b.val;
            auto sqrMag = dba.sqrMagnitude();
            dba = dba.normalized();

            auto& vel_a = view.get<Velocity>(entity_a);
            auto& vel_b = view.get<Velocity>(entity_b);
            auto dot_a = dot(dba, vel_a.val);
            auto dot_b = dot(dba, vel_b.val);

            if (sqrMag < R_sqr && (dot_a - dot_b) < 0) {
                auto cr = 0.5f * (view.get<Restitution>(entity_a).val + view.get<Restitution>(entity_b).val);
                auto ma = view.get<Mass>(entity_a).val;
                auto mb = view.get<Mass>(entity_b).val;
                auto inv_sum = 1.0 / (ma + mb);
                auto ua = dot_a * dba;
                auto ub = dot_b * dba;
                auto va = inv_sum * (cr * mb *(ub - ua) + ma * ua + mb * ub);
                auto vb = inv_sum * (cr * ma * (ua - ub) + ma * ua + mb * ub);
                vel_a.val = vel_a.val - ua + va;
                vel_b.val = vel_b.val - ub + vb;
                auto& acc_a = view.get<Acceleration>(entity_a);
                auto& acc_b = view.get<Acceleration>(entity_b);
                auto da_a = -dot(acc_a.val,dba)*dba;
                acc_a.val = acc_a.val + da_a;
                auto da_b = -dot(acc_b.val,dba)*dba;
                acc_b.val = acc_b.val + da_b;
            }
        }
    }
}

