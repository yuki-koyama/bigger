#include <bigger/utils.hpp>
#include <stdexcept>

std::string bigger::getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type)
{
    switch (renderer_type)
    {
        case bgfx::RendererType::Noop:                               break;
        case bgfx::RendererType::Direct3D9:  return "shaders/dx9";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: return "shaders/dx11";  break;
        case bgfx::RendererType::Gnm:                                break;
        case bgfx::RendererType::Metal:      return "shaders/metal"; break;
        case bgfx::RendererType::OpenGL:     return "shaders/glsl";  break;
        case bgfx::RendererType::OpenGLES:   return "shaders/essl";  break;
        case bgfx::RendererType::Vulkan:                             break;
        case bgfx::RendererType::Count:                              break;
    }
    throw std::runtime_error("Renderer type not supported.");
}
