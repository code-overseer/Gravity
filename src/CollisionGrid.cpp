#include "../include/CollisionGrid.hpp"
#include "../include/Utils.hpp"
#include "../include/components.hpp"
#include <set>

using namespace gravity;

CollisionGrid::CollisionGrid(AABB world, int width, int height, int particles) : _width(width), _height(height),
                                                                                 _world(std::move(world)), _tightCells(width * height),
                                                                                 _looseCells(width * height) {
    int const size = width * height;
    _looseNodes.reserve(size);
    _entities.reserve(particles);
}

void CollisionGrid::add(entt::entity const &e, mathsimd::float2 const &pos, components::CircleCollider const&radius) {
    if (!_world.contains(pos)) return;
    int const idx = getIndex(pos, _world, static_cast<float>(_width), static_cast<float>(_height));
    auto this_box = radius.toAABB(pos);
    int next_ent = _looseCells[idx].head;
    _looseCells[idx].head = _entities.size();
    _looseCells[idx].count++;
    _entities.emplace_back(e, next_ent);
    _looseCells[idx].box.min = mathsimd::float2::minimum( _looseCells[idx].box.min, this_box.min);
    _looseCells[idx].box.max = mathsimd::float2::maximum( _looseCells[idx].box.max, this_box.max);
}

void CollisionGrid::flushLooseCells() {
    for (size_t c = 0; c < _looseCells.size(); ++c) {
        auto const & cell = _looseCells[c];
        if (cell.head == -1) continue;
        auto tr = getIndex(cell.box.max, _world, static_cast<float>(_width), static_cast<float>(_height));
        volatile auto bl = getIndex(cell.box.min, _world, static_cast<float>(_width), static_cast<float>(_height));

        auto x0 = bl % _width, x1 = tr % _width;
        auto y0 = bl / _width, y1 = tr / _width;


        for (auto i = y0; i <= y1; ++i) {
            for (auto j = x0; j <= x1; ++j) {
                auto t_cell = i * _width + j;
                int next_node = _tightCells[t_cell].head;
                _tightCells[t_cell].head = _looseNodes.size();
                _tightCells[t_cell].count++;
                _looseNodes.emplace_back(c, next_node);
            }
        }

    }
}

void CollisionGrid::clear() {
    _tightCells.clear();
    _looseCells.clear();
    _looseNodes.clear();
    _entities.clear();
    _tightCells.resize(_width * _height, TightCell());
    _looseCells.resize(_width * _height, LooseCell());
}

std::vector<entt::entity> & CollisionGrid::query(AABB const &box) {
    _queryResult.clear();
    if (!_world.contains(box.max) || !_world.contains(box.min)) return _queryResult;
    auto tr = getIndex(box.max,_world, static_cast<float>(_width), static_cast<float>(_height));
    auto bl = getIndex(box.min,_world, static_cast<float>(_width), static_cast<float>(_height));
    auto x0 = bl % _width, x1 = tr % _width;
    auto y0 = bl / _width, y1 = tr / _width;

    for (auto i = y0; i <= y1; ++i) {
        for (auto j = x0; j <= x1; ++j) {
            auto t_cell = i * _width + j;
            int stop_cell = _tightCells[t_cell].count - 1;
            for (auto l_node = _tightCells[t_cell].head; l_node != -1 && stop_cell >= 0; l_node = _looseNodes[l_node].next, --stop_cell) {
                 auto l_cell = _looseNodes[l_node].cell;
                 int stop_e = _looseCells[l_cell].count - 1;
                 for (auto e = _looseCells[l_cell].head; e != -1 && stop_e >= 0; e = _entities[e].next, --stop_e) {
                     _queryResult.emplace_back(_entities[e].val);
                 }
            }
        }
    }
    return _queryResult;
}
