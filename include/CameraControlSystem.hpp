

#ifndef GRAVITY_CAMERACONTROLSYSTEM_HPP
#define GRAVITY_CAMERACONTROLSYSTEM_HPP
#include "System.hpp"

namespace gravity::systems {

    class CameraControlSystem : public System {
        friend class gravity::World;
        CameraControlSystem(gravity::World& w) : System(w) {};
    };
}


#endif //GRAVITY_CAMERACONTROLSYSTEM_HPP
