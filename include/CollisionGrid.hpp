#ifndef GRAVITY_COLLISIONGRID_HPP
#define GRAVITY_COLLISIONGRID_HPP

#include <mathsimd.hpp>
#include <entt/entt.hpp>
#include "AABB.hpp"
#include "components.hpp"
#include <utility>
#include <limits>

namespace gravity {
    struct CollisionGrid {
        struct LooseCell {
        private:
            template<typename T>
            using __lim = std::numeric_limits<T>;
        public:
            AABB box{__lim<float>::max(), __lim<float>::max(), -__lim<float>::max(), -__lim<float>::max()};
            int head = -1;
            int count = 0;
            LooseCell() = default;
        };
        struct LooseNode {
            int cell = -1;
            int next = -1;
            explicit LooseNode(int cell, int next = -1) : cell(cell), next(next) {}
        };
        struct TightCell {
            int head = -1;
            int count = 0;
            TightCell() = default;
        };
        struct ElementNode {
            entt::entity val;
            int next = -1;
            explicit ElementNode(entt::entity const& entity, int next = -1) : val(entity), next(next) {}
        };
        int _width = 0;
        int _height = 0;
        AABB _world;
        std::vector<ElementNode> _entities;
        std::vector<LooseCell> _looseCells;
        std::vector<LooseNode> _looseNodes;
        std::vector<TightCell> _tightCells;
        CollisionGrid() = default;
        CollisionGrid(AABB world, int width, int height, int particles);
        CollisionGrid(CollisionGrid&& grid) noexcept : _entities(std::move(grid._entities)), _looseCells(std::move(grid._looseCells)),
        _looseNodes(std::move(grid._looseNodes)), _tightCells(std::move(grid._tightCells)),
        _width(grid._width), _height(grid._height), _world(std::move(grid._world)) {}
        CollisionGrid &operator=(CollisionGrid&& grid) noexcept {
            _entities= std::move(grid._entities);
            _looseCells= std::move(grid._looseCells);
            _looseNodes= std::move(grid._looseNodes);
            _tightCells= std::move(grid._tightCells);
            _width = grid._width;
            _height = grid._height;
            _world = std::move(grid._world);
            return *this;
        }
        void add(entt::entity const &e, mathsimd::float2 const &pos, components::CircleCollider const&radius);
        [[nodiscard]] std::vector<entt::entity> const& query(AABB const& box);
        void flushLooseCells();
        void clear();
    private:
        std::vector<entt::entity> _queryResult;
    };


}

#endif //GRAVITY_COLLISIONGRID_HPP
