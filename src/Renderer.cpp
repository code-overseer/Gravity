#include "../include/Renderer.hpp"
#include "../include/Utils.hpp"

gravity::Renderer::Renderer() : mtl_cpp::Metal_API(), _device(getDevice()), _shaderCode(readFile(SHADER_SOURCE)) {
    _sema = dispatch_semaphore_create(BUFFER_SIZE);
}

void gravity::Renderer::onInitialize(void *view) {
    _lib = compileLibrary(_device, _shaderCode.data(), true);
    _queue = createCommandQueue(_device);
    _shader = createRenderState(view, _lib, _device, 4, "vertexShader", "fragmentShader");
}

void gravity::Renderer::onDraw(void *view) {
    static auto handler = [this]() { dispatch_semaphore_signal(_sema); };
    if (!_instanceCount) return;
    double target = getTime() + 0.016666667;

    dispatch_semaphore_wait(_sema, DISPATCH_TIME_FOREVER);
    auto buffer = getCommandBuffer(_queue);
    addCompletionHandler(buffer, handler);

    /* Update dynamic buffers */
    _localToWorlds[_buffer_idx].flush();
    _camera[_buffer_idx].flush();
    /* End */

    auto encoder = getRenderCommandEncoder(view, buffer, _sampling);
    setState(encoder, _shader);
    setVertexBuffer(encoder, _vertices, 0, 0);
    setVertexBuffer(encoder, _localToWorlds[_buffer_idx], 0, 1);
    setVertexBuffer(encoder, _camera[_buffer_idx], 0, 0);
    drawMesh(encoder, mtl_cpp::Triangle, _triangles.length<uint16_t>(),
            mtl_cpp::UINT16, _triangles, 0, _instanceCount, 0, 0);
    endEncoding(encoder);
    presentDrawable(view, buffer, target);
    commitCommandBuffer(buffer);
    _buffer_idx = (_buffer_idx + 1) % BUFFER_SIZE;
}

void gravity::Renderer::onSizeChange(void *view, unsigned long const size[2]) {
    _sampling = createMultiSamplingTexture(view, _device, size[0], size[2], 4);
}
