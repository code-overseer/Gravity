#ifndef GRAVITY_SYSTEM_HPP
#define GRAVITY_SYSTEM_HPP

#include <entt/entt.hpp>
#include <type_traits>
namespace gravity {
    class World;

    namespace systems {
        struct System {
        private:
            World& _world;
        protected:
            System(World& w) : _world(w) {}

            World& world() { return _world; }
        public:
            virtual ~System() {}
            System(System const&) = delete;
            virtual void update(float delta) = 0;
        };

    }
}




#endif //GRAVITY_SYSTEM_HPP
