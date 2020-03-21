

#ifndef GRAVITY_CAMERACONTROLSYSTEM_HPP
#define GRAVITY_CAMERACONTROLSYSTEM_HPP
#include "System.hpp"
#include <mathsimd.hpp>
extern "C" {
#include <Cocoa_API.h>
};
namespace gravity::systems {

    class CameraControlSystem : public System {
        friend class gravity::World;
        CameraControlSystem(gravity::World& w);
        mathsimd::float2 _translationSpeed;
        float _zoomSpeed;
        float _zoomLimits[2];

        float horizontal() { return INPUT ? INPUT->horizontal : 0; }
        float vertical() { return INPUT ? INPUT->vertical : 0; }
        float scrollX() { return INPUT ? INPUT->scroll_x : 0; }
        float scrollY() { return INPUT ? INPUT->scroll_y : 0; }
        float magnify() { return INPUT ? INPUT->magnify : 0; }
    public:
        void update(float delta) override;
    };
}


#endif //GRAVITY_CAMERACONTROLSYSTEM_HPP
