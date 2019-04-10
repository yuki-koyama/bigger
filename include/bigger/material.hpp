#ifndef material_hpp
#define material_hpp

#include <string>
#include <bigger/bigger.hpp>

namespace bigger
{
    class Material
    {
    public:
        Material(const std::string& shader_name)
        {
            const std::string shader_dir_path = getShaderDirectoryPath(bgfx::getRendererType());
            const std::string vs_path = shader_dir_path + "/" + "vs_" + shader_name + ".bin";
            const std::string fs_path = shader_dir_path + "/" + "fs_" + shader_name + ".bin";

            m_program = bigg::loadProgram(vs_path.c_str(), fs_path.c_str());
        }

        ~Material()
        {
            bgfx::destroy(m_program);
        }

        bgfx::ProgramHandle m_program;
    };
}

#endif /* material_hpp */