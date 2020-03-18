

#ifndef GRAVITY_GRAVITYSYSTEM_HPP
#define GRAVITY_GRAVITYSYSTEM_HPP
#include "System.hpp"
#include "AABB.hpp"
#include "BH_Grid.hpp"

namespace gravity::systems {

    class GravitySystem : public System {
    private:
        BH_Grid<4,4> _grid;
    public:
        GravitySystem(entt::registry *reg, AABB const&world, int entities, float theta_sqr);
        void update(float delta) override;
    };
}



#endif //GRAVITY_GRAVITYSYSTEM_HPP
