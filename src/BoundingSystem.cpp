#include "../include/BoundingSystem.hpp"
#include "../include/components.hpp"
#include "../include/AABB.hpp"
#include "../include/World.hpp"

void gravity::systems::BoundingSystem::update(float delta) {
    using namespace gravity::components;
    auto b = world().bounds();
    world().registry().view<Position,Velocity, CircleCollider>().each([b](Position const &p, Velocity& v, CircleCollider const& c) {
        auto tr = p.val + c.radius;
        auto bl = p.val - c.radius;
        bool r = tr.x() > b.max.x(), l = bl.x() < b.min.x(), t = tr.y() > b.max.y(), btm = bl.y() < b.min.y();
        auto tmp = mathsimd::float2(((r && v.val.x() > 0) || (l && v.val.x() < 0)),
                                    ((t && v.val.y() > 0) || (btm && v.val.y() < 0)));
        v.val = v.val - 2 * tmp * v.val;
    });
}
