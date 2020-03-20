

#include <taskflow.hpp>
#include "../include/For_.hpp"
#include "../include/components.hpp"
#include "../include/GravitySystem.hpp"
#include "../include/World.hpp"

namespace gsys = gravity::systems;


gsys::GravitySystem::GravitySystem(gravity::World &w, int entities, float theta_sqr) : System(w), _grid(w.bounds(), entities, theta_sqr) {
    using namespace gravity::components;
    using Reg = entt::registry;

}

void gsys::GravitySystem::update(float delta) {
    using namespace gravity::components;
    _grid.clear();
    _insertToTrees();
    tf::Taskflow tf;
    auto view = world().registry().view<Acceleration, Position, Velocity, Mass>();

    for (auto entity : view) {
        auto pos = view.get<Position>(entity);
        auto new_acc = mathsimd::float2::zero();
        for (int i = 0; i < decltype(_grid)::N; ++i) {
            auto& tree =  _grid.tree(i);
            new_acc = new_acc + tree.traverse(pos);
        }

        auto& acc = view.get<Acceleration>(entity);
        auto& vel = view.get<Velocity>(entity);
        vel.val = vel.val + (acc.val + new_acc)*(0.5f * delta);
        acc.val = new_acc;
    }
}

void gravity::systems::GravitySystem::_insertToTrees() {
    using namespace components;
    tf::Taskflow tf;

    for_<decltype(_grid)::N>([&](auto i) {
        using Tp = entt::tag<decltype(_grid)::CellTag(decltype(i)::value)>;
        auto group = world().registry().group<Tp>(entt::get<Position, Mass>);
        _insertion[decltype(i)::value].size = group.size();
        _insertion[decltype(i)::value].tree = &_grid.tree(decltype(i)::value);
        _insertion[decltype(i)::value].entity = group.template data<Tp>();
    });

    tf.parallel_for(_insertion.begin(), _insertion.end(), [view = world().registry().view<Position,Mass>()](insertion_arg_t& args) {
        for (int i = 0; i < args.size; ++i) args.tree->insert(view.get<Position>(args.entity[i]), view.get<Mass>(args.entity[i])); });

    world().executor().run(tf).wait();
}
