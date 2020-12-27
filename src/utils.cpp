#include <bigger/utils.hpp>
#include <stdexcept>

std::string bigger::getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type)
{
    switch (renderer_type)
    {
    case bgfx::RendererType::Direct3D9:
        return "shaders/dx9";
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        return "shaders/dx11";
    case bgfx::RendererType::Metal:
        return "shaders/metal";
    case bgfx::RendererType::OpenGL:
        return "shaders/glsl";
    case bgfx::RendererType::OpenGLES:
        return "shaders/essl";
    case bgfx::RendererType::Nvn:
    case bgfx::RendererType::Gnm:
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::Count:
        break;
    }
    throw std::runtime_error("Renderer type not supported.");
}
