#ifndef GRAVITY_SYSTEM_HPP
#define GRAVITY_SYSTEM_HPP

#include <entt/entt.hpp>
namespace gravity::systems {
    class System {
    protected:
        explicit System(entt::registry* reg) : _registry(reg) {}
        System(System && other) noexcept : _registry(std::move(other._registry)) { other._registry = nullptr; }
        entt::registry* _registry = nullptr;
    public:
        virtual ~System() { _registry = nullptr; }
        virtual void update(float delta) = 0;
    };
}


#endif //GRAVITY_SYSTEM_HPP
