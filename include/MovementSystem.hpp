

#ifndef GRAVITY_MOVEMENTSYSTEM_HPP
#define GRAVITY_MOVEMENTSYSTEM_HPP

#include <entt/entt.hpp>
#include <unordered_map>
#include <functional>
#include "System.hpp"
#include "BH_Grid.hpp"
#include "World.hpp"

namespace gravity::systems {

    class MovementSystem : public System {
    private:
        entt::dispatcher _eventDispatcher;
        std::unordered_map<int, std::function<void(entt::entity)>> _moveTo;
        std::unordered_map<int, std::function<void(entt::entity)>> _moveFrom;
        friend class gravity::World;
        MovementSystem(gravity::World& w);
    public:
        void update(float delta) override;
        template <int CELL>
        struct MoveToCellEvent {
            entt::entity entity;
            MoveToCellEvent(entt::entity const& e) : entity(e) {}
        };
        template <int CELL>
        struct MoveFromCellEvent {
            entt::entity entity;
            MoveFromCellEvent(entt::entity const& e) : entity(e) {}
        };
        template <int CELL>
        void MoveToCell(MoveToCellEvent<CELL> const & event) {
            world().registry().assign<entt::tag<BH_Grid<4,4>::CellTag(CELL)>>(event.entity);
        }
        template <int CELL>
        void MoveFromCell(MoveFromCellEvent<CELL> const& event) {
            world().registry().remove_if_exists<entt::tag<BH_Grid<4,4>::CellTag(CELL)>>(event.entity);
        }

    };
}



#endif //GRAVITY_MOVEMENTSYSTEM_HPP
