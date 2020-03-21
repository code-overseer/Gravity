#include "../include/RenderSystem.hpp"
#include "../include/Utils.hpp"
#include "../include/Mesh.hpp"
#include "../include/World.hpp"
#include "../include/components.hpp"
#include <chrono>

gravity::systems::RenderSystem::RenderSystem(World& w) : _renderer(), System(w) {
    using namespace gravity::components;
    w.registry().group<Position, CircleCollider, LocalToWorld>();
    addResizeListener([this](unsigned long const size[2]) {
        world().mainCamera().set_aspect_ratio(static_cast<float>(size[0]), static_cast<float>(size[1]));
    });
}

gravity::systems::RenderSystem::RenderAPI::RenderAPI() : mtl_cpp::Metal_API(), device(getDevice()), shaderCode(readFile(SHADER_SOURCE)) {
    sema = dispatch_semaphore_create(BUFFER_SIZE);
}

void gravity::systems::RenderSystem::RenderAPI::onInitialize(void *view) {
    lib = compileLibrary(device, shaderCode.data(), true);
    queue = createCommandQueue(device);
    shader = createRenderState(view, lib, device, 4, "vertexShader", "fragmentShader");
    auto m = Mesh::makeCircle();
    vertices = mallocBuffer(device, m.vertices.data(), m.vertices.size() * sizeof(mathsimd::float2), mtl_cpp::Managed);
    triangles = mallocBuffer(device, m.triangles.data(), m.triangles.size() * sizeof(uint16_t), mtl_cpp::Managed);
}

void gravity::systems::RenderSystem::RenderAPI::onDraw(void *view) {
    if (!instanceCount) return;
    using namespace std::chrono;
    const double target = getTime() + 0.016667;
    static auto handler = [this]() {
        dispatch_semaphore_signal(sema);
    };

    dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
    auto buffer = getCommandBuffer(queue);
    addCompletionHandler(buffer, handler);

    /* Update dynamic buffers */
    localToWorlds[buffer_idx].flush();
    /* End */
    auto encoder = getRenderCommandEncoder(view, buffer, sampling);
    setState(encoder, shader);

    setVertexBuffer(encoder, vertices, 0, 0);
    setVertexBuffer(encoder, localToWorlds[buffer_idx], 0, 1);
    setVertexBuffer(encoder, camera[buffer_idx], 0, 2);
    drawMesh(encoder, mtl_cpp::Triangle, triangles.length<uint16_t>(),
             mtl_cpp::UINT16, triangles, 0, instanceCount);

    endEncoding(encoder);
    presentDrawable(view, buffer, target);
    commitCommandBuffer(buffer);
    buffer_idx = (buffer_idx + 1) % BUFFER_SIZE;
}

void gravity::systems::RenderSystem::RenderAPI::onSizeChange(void *view, unsigned long const size[2]) {
    sampling = createMultiSamplingTexture(view, device, size[0], size[1], 4);
    for (auto &func : resizeListeners) func(size);
}

void gravity::systems::RenderSystem::update(float delta) {
    using namespace gravity::components;
    using namespace mtl_cpp;
    auto tmp = camera_t(world().mainCamera());
    if (_renderer.camera[_renderer.buffer_idx].isEmpty()) {
        _renderer.camera[_renderer.buffer_idx] = Metal_API::mallocBuffer(_renderer.device, &tmp, sizeof(camera_t), mtl_cpp::Shared);
    } else {
        _renderer.camera[_renderer.buffer_idx].copy(&tmp, sizeof(camera_t));
    }

    auto group = world().registry().group<const Position, const CircleCollider, LocalToWorld>();
    group.each([](Position const&pos, CircleCollider const& col, LocalToWorld& ltw) {
        ltw = components::LocalToWorld::fromPositionAndRadius(pos.val, col.radius);
    });

    // update ltw buffer
    auto &buffer = _renderer.localToWorlds[_renderer.buffer_idx];
    if (buffer.isEmpty() || buffer.length<LocalToWorld>() != group.size<LocalToWorld>()) {
        buffer = Metal_API::mallocBuffer(_renderer.device, group.raw<LocalToWorld>(), group.size() * sizeof(LocalToWorld), mtl_cpp::Managed);
    } else {
        buffer.copy(group.raw<LocalToWorld>(), group.size() * sizeof(components::LocalToWorld));
    }
    _renderer.instanceCount = group.size();
}

void gravity::systems::RenderSystem::addResizeListener(std::function<void(unsigned long const size[2])> &&delegate) {
    _renderer.resizeListeners.emplace_back(std::forward<decltype(delegate)>(delegate));
}
