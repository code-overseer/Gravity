#include "../include/CameraControlSystem.hpp"
#include "../include/World.hpp"

gravity::systems::CameraControlSystem::CameraControlSystem(gravity::World &w) : System(w), _zoomLimits{1,1} {
    auto s = world().bounds().size();
    _translationSpeed = s * 0.25f;
    auto m = s.magnitude();
    _zoomSpeed = m * 0.25f;
    _zoomLimits[0] = 50;
    _zoomLimits[1] = m;
}

void gravity::systems::CameraControlSystem::update(float delta) {
    auto& cam = world().mainCamera();

    cam.position(cam.position() + mathsimd::float2(horizontal(),vertical()) * _translationSpeed * delta);
    cam.position(cam.position() + mathsimd::float2(scrollX(),-scrollY()) * _translationSpeed * delta);

    auto z = cam.zoom();
    z += magnify() * _zoomSpeed * delta;
    cam.zoom(std::clamp(z, _zoomLimits[0], _zoomLimits[1]));

}