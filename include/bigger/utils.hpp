#ifndef BIGGER_UTILS_HPP
#define BIGGER_UTILS_HPP

#include <bgfx/bgfx.h>
#include <string>

namespace bigger
{
    std::string getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type);
}

#endif // BIGGER_UTILS_HPP
