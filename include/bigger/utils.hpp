#ifndef BIGGER_UTILS_HPP
#define BIGGER_UTILS_HPP

#include <string>
#include <bgfx/bgfx.h>

namespace bigger
{
    std::string getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type);
}

#endif // BIGGER_UTILS_HPP
