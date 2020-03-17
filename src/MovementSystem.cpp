

#include "../include/MovementSystem.hpp"
#include "../include/components.hpp"

void gravity::systems::MovementSystem::update(float delta) {
    using namespace gravity::components;
    auto view = _registry->view<Position,components::Velocity, components::Acceleration>();
    view.each([=](Position &p, Velocity const& v, Acceleration const& a) {
        p.val = p.val + v.val * delta + a.val * delta * delta * 0.5f;
    });
}

