#ifndef BIGGER_BLINNPHONG_MATERIAL_HPP
#define BIGGER_BLINNPHONG_MATERIAL_HPP

#include <array>
#include <bgfx/bgfx.h>
#include <bigger/material.hpp>
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
            m_handle_params        = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, num_vec4_uniforms);
            m_handle_tex_transform = bgfx::createUniform("u_tex_transform", bgfx::UniformType::Mat3);
            m_handle_tex_color     = bgfx::createUniform("s_tex_color", bgfx::UniformType::Sampler);

            constexpr bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA32F;
            constexpr uint64_t                  flags  = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;

            m_tex_color_alt = bgfx::createTexture2D(1, 1, false, 1, format, flags, nullptr);

#if BX_PLATFORM_OSX
            // This is an ad-hoc hotfix to avoid the Metal API Validation assertion; see
            // https://github.com/yuki-koyama/bigger/issues/13
            bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
#endif
        }

        ~BlinnPhongMaterial()
        {
            bgfx::destroy(m_handle_params);
            bgfx::destroy(m_handle_tex_color);
            bgfx::destroy(m_handle_tex_transform);

            bgfx::destroy(m_tex_color_alt);
        }

        void setTextureColor(bgfx::TextureHandle tex_color)
        {
            assert(isValid(tex_color));

            m_tex_color   = tex_color;
            m_is_textured = true;
        }

        void submitUniforms() override
        {
            // Set uniform parameters (in a packed way)
            constexpr float dummy = 0.0f;

            const std::array<glm::vec4, num_vec4_uniforms> buffer = {{
                {u_diffuse, dummy},
                {u_specular, dummy},
                {u_ambient, u_shininess},
                {u_dir_lights[0].dir, dummy},
                {u_dir_lights[0].color, dummy},
                {u_dir_lights[1].dir, dummy},
                {u_dir_lights[1].color, dummy},
            }};
            bgfx::setUniform(m_handle_params, buffer.data(), num_vec4_uniforms);

            // Set the transform matrix for texture coordinates
#if false
            constexpr float texture_transform[] =
                {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}; // Do nothing
#else
            constexpr float texture_transform[] =
                {1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f}; // Flip vertically
#endif
            bgfx::setUniform(m_handle_tex_transform, texture_transform);

            // Set the diffuse color
            if (m_is_textured)
            {
                bgfx::setTexture(0, m_handle_tex_color, m_tex_color);
            }
            else
            {
                // Create and set a 1x1 small diffuse color texture
                static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

                color[0] = u_diffuse.r;
                color[1] = u_diffuse.g;
                color[2] = u_diffuse.b;

                const bgfx::Memory* mem = bgfx::makeRef(color, sizeof(float) * 4);

                // Update the texture with the current color
                bgfx::updateTexture2D(m_tex_color_alt, 0, 0, 0, 0, 1, 1, mem);
                bgfx::setTexture(0, m_handle_tex_color, m_tex_color_alt);
            }
        }

        void drawImgui() override
        {
            ImGui::Text("Material Setting");
            if (m_is_textured)
            {
                ImGui::Text("diffuse (textured)");
            }
            else
            {
                ImGui::ColorEdit3("diffuse", glm::value_ptr(u_diffuse));
            }
            ImGui::ColorEdit3("specular", glm::value_ptr(u_specular));
            ImGui::ColorEdit3("ambient", glm::value_ptr(u_ambient));
            ImGui::SliderFloat("shininess", &u_shininess, 0.5f, 256.0f);
            ImGui::SliderFloat3("dir_light_0_dir", glm::value_ptr(u_dir_lights[0].dir), -1.0f, 1.0f);
            ImGui::ColorEdit3("dir_light_0_color", glm::value_ptr(u_dir_lights[0].color));
            ImGui::SliderFloat3("dir_light_1_dir", glm::value_ptr(u_dir_lights[1].dir), -1.0f, 1.0f);
            ImGui::ColorEdit3("dir_light_1_color", glm::value_ptr(u_dir_lights[1].color));
        }

        glm::vec3 u_diffuse   = glm::vec3(0.78f, 0.71f, 0.85f);
        glm::vec3 u_specular  = glm::vec3(1.00f, 1.00f, 1.00f);
        glm::vec3 u_ambient   = glm::vec3(0.00f, 0.00f, 0.00f);
        float     u_shininess = 128.0f;

        std::array<DirLight, 2> u_dir_lights = {{
            DirLight{{+0.5, +0.5, +1.0}, {1.0, 0.9, 0.9}},
            DirLight{{-1.0, +0.0, +0.0}, {0.2, 0.2, 0.5}},
        }};

    private:
        /// \brief Boolean indicating whether a diffuse texture is set by another class or not.
        bool m_is_textured = false;

        // -------------------------------------------------------------------------------------------------------------
        // Handles managed by this class.
        // -------------------------------------------------------------------------------------------------------------

        bgfx::UniformHandle m_handle_params;
        bgfx::UniformHandle m_handle_tex_color;
        bgfx::UniformHandle m_handle_tex_transform;

        /// \brief Texture handle that will be used when no diffuse texture is set.
        bgfx::TextureHandle m_tex_color_alt = BGFX_INVALID_HANDLE;

        // -------------------------------------------------------------------------------------------------------------
        // Handles not managed by this class.
        // -------------------------------------------------------------------------------------------------------------

        /// \brief Texture handle for diffuse color, which should be set and managed by another class.
        bgfx::TextureHandle m_tex_color = BGFX_INVALID_HANDLE;
    };
} // namespace bigger

#endif // BIGGER_BLINNPHONG_MATERIAL_HPP
