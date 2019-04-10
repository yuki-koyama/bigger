#ifndef utils_hpp
#define utils_hpp

#include <string>
#include <bgfx/bgfx.h>

namespace bigger
{
    std::string getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type);
}

#endif /* utils_hpp */
