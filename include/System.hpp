#ifndef GRAVITY_SYSTEM_HPP
#define GRAVITY_SYSTEM_HPP

#include <entt/entt.hpp>
namespace gravity::systems {
    class System {
    protected:
        explicit System(entt::registry* reg) : _registry(reg) {}
        entt::registry* _registry = nullptr;
    public:
        virtual void update(float delta) = 0;
    };
}


#endif //GRAVITY_SYSTEM_HPP
