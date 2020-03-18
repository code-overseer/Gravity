

#include "../include/GravitySystem.hpp"
namespace gsys = gravity::systems;

gsys::GravitySystem::GravitySystem(entt::registry *reg, const gravity::AABB &world, int entities, float theta_sqr) : System(reg), _grid(world, entities, theta_sqr) {

}

void gsys::GravitySystem::update(float delta) {

}

