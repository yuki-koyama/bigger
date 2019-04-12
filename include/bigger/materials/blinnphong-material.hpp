#ifndef blinnphong_material_hpp
#define blinnphong_material_hpp

#include <bigger/material.hpp>
#include <array>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <imgui.h>

namespace bigger
{
    class BlinnPhongMaterial final : public Material
    {
    public:

        BlinnPhongMaterial() : bigger::Material("blinnphong")
        {
            m_handle = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, 3);
        }

        ~BlinnPhongMaterial()
        {
            bgfx::destroy(m_handle);
        }

        void submitUniforms() override
        {
            constexpr float dummy = 0.0f;

            const std::array<glm::vec4, 3> buffer =
            {{
                { u_diffuse, dummy },
                { u_specular, dummy },
                { u_ambient, u_shininess },
            }};
            bgfx::setUniform(m_handle, buffer.data(), 3);
        }

        void drawImgui() override
        {
            ImGui::SliderFloat3("diffuse", glm::value_ptr(u_diffuse), 0.0f, 1.0f);
            ImGui::SliderFloat3("specular", glm::value_ptr(u_specular), 0.0f, 1.0f);
            ImGui::SliderFloat3("ambient", glm::value_ptr(u_ambient), 0.0f, 1.0f);
            ImGui::SliderFloat("shininess", &u_shininess, 0.1f, 256.0f);
        }

        glm::vec3 u_diffuse = glm::vec3(0.5f, 0.4f, 0.6f);
        glm::vec3 u_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 u_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        float u_shininess = 128.0f;

    private:

        bgfx::UniformHandle m_handle;
    };
}

#endif /* blinnphong_material_hpp */
