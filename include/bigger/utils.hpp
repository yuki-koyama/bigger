#ifndef BIGGER_UTILS_HPP
#define BIGGER_UTILS_HPP

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <string>

namespace bigger
{
    std::string getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type);

    // A wrapper function to use utility functions in bgfx's examples
    bgfx::TextureHandle loadTexture(const char* file_path);
} // namespace bigger

#endif // BIGGER_UTILS_HPP
