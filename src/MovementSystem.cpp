#include <utility>
#include "../include/BH_Grid.hpp"
#include "../include/AABB.hpp"
#include "../include/Utils.hpp"
#include "../include/components.hpp"
#include "../include/MovementSystem.hpp"
#include "../include/For_.hpp"

gravity::systems::MovementSystem::MovementSystem(gravity::World& w) : System(w) {

    for_<BH_Grid<4,4>::N>([&] (auto i) {
        _eventDispatcher.sink<MoveFromCellEvent<decltype(i)::value>>().template connect<&MovementSystem::MoveFromCell<decltype(i)::value>>(*this);
        _eventDispatcher.sink<MoveToCellEvent<decltype(i)::value>>().template connect<&MovementSystem::MoveToCell<decltype(i)::value>>(*this);
        _moveFrom.emplace(std::make_pair(decltype(i)::value, std::bind(&entt::dispatcher::enqueue<MoveFromCellEvent<decltype(i)::value>, entt::entity>, &_eventDispatcher, std::placeholders::_1)));
        _moveTo.emplace(std::make_pair(decltype(i)::value, std::bind(&entt::dispatcher::enqueue<MoveToCellEvent<decltype(i)::value>, entt::entity>, &_eventDispatcher, std::placeholders::_1)));
    });

    using namespace gravity::components;
    auto v = w.registry().view<const Position>();
    for (auto entity : v) {
        auto p = v.get<const Position>(entity);
        auto idx = getIndex(p.val, w.bounds(), BH_Grid<4,4>::W, BH_Grid<4,4>::H);
        _moveTo.at(idx)(entity);
    }
    for_<BH_Grid<4,4>::N>([&] (auto i) {
        _eventDispatcher.update<MoveToCellEvent<decltype(i)::value>>();
    });
}

void gravity::systems::MovementSystem::update(float delta) {
    using namespace gravity::components;
    auto view = world().registry().view<Position, const Velocity, const Acceleration>();

    for (auto entity : view) {
        auto& pos = view.get<Position>(entity);
        auto vel = view.get<const Velocity>(entity);
        auto acc = view.get<const Acceleration>(entity);
        auto previous = getIndex(pos.val, world().bounds(), BH_Grid<4,4>::W, BH_Grid<4,4>::H);
        pos.val = pos.val + vel.val * delta + acc.val * delta * delta * 0.5f;
        auto current = getIndex(pos.val, world().bounds(), BH_Grid<4,4>::W, BH_Grid<4,4>::H);
        if (current != previous && _moveTo.count(current)) {
            _moveFrom.at(previous)(entity);
            _moveTo.at(current)(entity);
        }
    }
    for_<BH_Grid<4,4>::N>([&] (auto i) {
        _eventDispatcher.update<MoveFromCellEvent<decltype(i)::value>>();
    });
    for_<BH_Grid<4,4>::N>([&] (auto i) {
        _eventDispatcher.update<MoveToCellEvent<decltype(i)::value>>();
    });
}


