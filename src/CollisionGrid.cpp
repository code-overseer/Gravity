#include "../include/CollisionGrid.hpp"
#include "../include/Utils.hpp"
#include "../include/components.hpp"
#include <set>

using namespace gravity;

CollisionGrid::CollisionGrid(AABB world, int width, int height, int particles) : _width(width), _height(height),
                                                                                 _world(std::move(world)), _tightCells(width * height) {
    int const size = width * height;
    _looseCells.reserve(size);
    _looseNodes.reserve(size);
    _entities.reserve(particles);
}

void CollisionGrid::add(entt::entity const &e, mathsimd::float2 const &pos, components::CircleCollider const&radius) {
    int const idx = getIndex(pos, _world, static_cast<float>(_width), static_cast<float>(_height));
    auto this_box = radius.toAABB(pos);
    int loose_node = -1;
    int loose_cell = -1;
    if (_tightCells[idx].head == -1) {
        loose_node = _tightCells[idx].head = _looseNodes.size();
        _looseNodes.emplace_back();
        loose_cell = _looseNodes.back().cell = _looseCells.size();
        _looseCells.emplace_back();
        _looseCells.back().box = this_box;
        _looseCells.back().head = _entities.size();
    } else {
        loose_node = _tightCells[idx].head;
        loose_cell = _looseNodes[loose_node].cell;
         _looseCells[loose_cell].box.min = mathsimd::float2::minimum( _looseCells[loose_cell].box.min, this_box.min);
         _looseCells[loose_cell].box.max = mathsimd::float2::maximum( _looseCells[loose_cell].box.max, this_box.max);
        auto entity_idx =  _looseCells[loose_cell].head; 
        while (_entities[entity_idx].next != -1) {
            entity_idx = _entities[entity_idx].next;
        }
        _entities[entity_idx].next = _entities.size();
    }
    _entities.emplace_back(e);
    std::set<int> loose_box_verts{
            getIndex(_looseCells[loose_cell].box.min, _world, static_cast<float>(_width), static_cast<float>(_height)),
            getIndex(_looseCells[loose_cell].box.max, _world, static_cast<float>(_width), static_cast<float>(_height)),
            getIndex({_looseCells[loose_cell].box.min.x(), _looseCells[loose_cell].box.max.y()}, _world, static_cast<float>(_width), static_cast<float>(_height)),
            getIndex({_looseCells[loose_cell].box.max.x(), _looseCells[loose_cell].box.min.y()}, _world, static_cast<float>(_width), static_cast<float>(_height))
    };
    
    for (auto const &i : loose_box_verts) {
        auto j = _tightCells[i].head;
        while (_looseNodes[j].next != -1 && _looseNodes[j].next != loose_node) {
            j = _looseNodes[j].next;
        }
        _looseNodes[j].next = loose_node;
    }
    
    

}