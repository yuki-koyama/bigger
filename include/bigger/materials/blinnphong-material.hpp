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

        static constexpr unsigned int num_vec4_uniforms = 7;

        struct DirLight
        {
            glm::vec3 dir;
            glm::vec3 color;
        };

        BlinnPhongMaterial() : bigger::Material("blinnphong")
        {
            m_handle = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, num_vec4_uniforms);
        }

        ~BlinnPhongMaterial()
        {
            bgfx::destroy(m_handle);
        }

        void submitUniforms() override
        {
            constexpr float dummy = 0.0f;

            const std::array<glm::vec4, num_vec4_uniforms> buffer =
            {{
                { u_diffuse, dummy },
                { u_specular, dummy },
                { u_ambient, u_shininess },
                { u_dir_lights[0].dir, dummy },
                { u_dir_lights[0].color, dummy },
                { u_dir_lights[1].dir, dummy },
                { u_dir_lights[1].color, dummy },
            }};
            bgfx::setUniform(m_handle, buffer.data(), num_vec4_uniforms);
        }

        void drawImgui() override
        {
            ImGui::SliderFloat3("diffuse", glm::value_ptr(u_diffuse), 0.0f, 1.0f);
            ImGui::SliderFloat3("specular", glm::value_ptr(u_specular), 0.0f, 1.0f);
            ImGui::SliderFloat3("ambient", glm::value_ptr(u_ambient), 0.0f, 1.0f);
            ImGui::SliderFloat("shininess", &u_shininess, 0.1f, 256.0f);
            ImGui::SliderFloat3("dir_light_0_dir", glm::value_ptr(u_dir_lights[0].dir), -1.0f, 1.0f);
            ImGui::SliderFloat3("dir_light_0_color", glm::value_ptr(u_dir_lights[0].color), 0.0f, 1.0f);
            ImGui::SliderFloat3("dir_light_1_dir", glm::value_ptr(u_dir_lights[1].dir), -1.0f, 1.0f);
            ImGui::SliderFloat3("dir_light_1_color", glm::value_ptr(u_dir_lights[1].color), 0.0f, 1.0f);
        }

        glm::vec3 u_diffuse = glm::vec3(0.5f, 0.4f, 0.6f);
        glm::vec3 u_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 u_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        float u_shininess = 128.0f;

        std::array<DirLight, 2> u_dir_lights =
        {{
            DirLight{ { + 0.5, + 0.5, + 1.0 }, { 1.0, 0.9, 0.9 } },
            DirLight{ { - 1.0, + 0.0, + 0.0 }, { 0.2, 0.2, 0.5 } },
        }};

    private:

        bgfx::UniformHandle m_handle;
    };
}

#endif /* blinnphong_material_hpp */
