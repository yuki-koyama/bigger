#ifndef BIGGER_UTILS_HPP
#define BIGGER_UTILS_HPP

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <string>

namespace bigger
{
    std::string getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type);

    // A wrapper function to use utility functions in bgfx's examples
    bgfx::TextureHandle loadTexture(const char*              name,
                                    uint64_t                 flags       = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
                                    uint8_t                  skip        = 0,
                                    bgfx::TextureInfo*       info        = NULL,
                                    bimg::Orientation::Enum* orientation = NULL);
} // namespace bigger

#endif // BIGGER_UTILS_HPP
