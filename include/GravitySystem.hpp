

#ifndef GRAVITY_GRAVITYSYSTEM_HPP
#define GRAVITY_GRAVITYSYSTEM_HPP
#include "System.hpp"
#include "BH_Grid.hpp"


namespace gravity::systems {

    class GravitySystem : public System {
        struct insertion_arg_t {
            gravity::BH_QuadTree* tree = nullptr;
            entt::entity const* entity = nullptr;
            std::size_t size = 0;
        };
    private:
        BH_Grid<4,4> _grid;
        friend class gravity::World;
        GravitySystem(gravity::World& w, int entities, float theta_sqr);
        std::array<insertion_arg_t, decltype(_grid)::N> _insertion;
        void _insertToTrees();

    public:
        void update(float delta) override;
    };
}



#endif //GRAVITY_GRAVITYSYSTEM_HPP
