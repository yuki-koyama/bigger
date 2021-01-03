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
        static constexpr unsigned int num_vec4_uniforms = 6;

        struct DirLight
        {
            glm::vec3 dir;
            glm::vec3 color;
        };

        BlinnPhongMaterial() : bigger::Material("blinnphong")
        {
            u_params        = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, num_vec4_uniforms);
            u_tex_transform = bgfx::createUniform("u_tex_transform", bgfx::UniformType::Mat3);
            s_tex_diffuse   = bgfx::createUniform("s_tex_diffuse", bgfx::UniformType::Sampler);

            constexpr bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA32F;
            constexpr uint64_t                  flags  = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;

            m_tex_diffuse_alt = bgfx::createTexture2D(1, 1, false, 1, format, flags, nullptr);

#if BX_PLATFORM_OSX
            // This is an ad-hoc hotfix to avoid the Metal API Validation assertion; see
            // https://github.com/yuki-koyama/bigger/issues/13
            bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
#endif
        }

        ~BlinnPhongMaterial()
        {
            bgfx::destroy(u_params);
            bgfx::destroy(u_tex_transform);
            bgfx::destroy(s_tex_diffuse);

            bgfx::destroy(m_tex_diffuse_alt);
        }

        void setTexDiffuse(bgfx::TextureHandle* tex_diffuse)
        {
            assert(isValid(*tex_diffuse));

            m_tex_diffuse = tex_diffuse;
            m_is_textured = true;
        }

        void submitUniforms() override
        {
            // Set uniform parameters (in a packed way)
            constexpr float dummy = 0.0f;

            const std::array<glm::vec4, num_vec4_uniforms> buffer = {{
                {m_specular, dummy},
                {m_ambient, m_shininess},
                {m_dir_lights[0].dir, dummy},
                {m_dir_lights[0].color, dummy},
                {m_dir_lights[1].dir, dummy},
                {m_dir_lights[1].color, dummy},
            }};
            bgfx::setUniform(u_params, buffer.data(), num_vec4_uniforms);

            // Set the transform matrix for texture coordinates
#if false
            constexpr float texture_transform[] =
                {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}; // Do nothing
#else
            constexpr float texture_transform[] =
                {1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f}; // Flip vertically
#endif
            bgfx::setUniform(u_tex_transform, texture_transform);

            // Set the diffuse color
            if (m_is_textured)
            {
                bgfx::setTexture(0, s_tex_diffuse, *m_tex_diffuse);
            }
            else
            {
                // Create and set a 1x1 small diffuse color texture
                static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

                color[0] = m_diffuse.r;
                color[1] = m_diffuse.g;
                color[2] = m_diffuse.b;

                const bgfx::Memory* mem = bgfx::makeRef(color, sizeof(float) * 4);

                // Update the texture with the current color
                bgfx::updateTexture2D(m_tex_diffuse_alt, 0, 0, 0, 0, 1, 1, mem);
                bgfx::setTexture(0, s_tex_diffuse, m_tex_diffuse_alt);
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
                ImGui::ColorEdit3("diffuse", glm::value_ptr(m_diffuse));
            }
            ImGui::ColorEdit3("specular", glm::value_ptr(m_specular));
            ImGui::ColorEdit3("ambient", glm::value_ptr(m_ambient));
            ImGui::SliderFloat("shininess", &m_shininess, 0.5f, 256.0f);
            ImGui::SliderFloat3("dir_light_0_dir", glm::value_ptr(m_dir_lights[0].dir), -1.0f, 1.0f);
            ImGui::ColorEdit3("dir_light_0_color", glm::value_ptr(m_dir_lights[0].color));
            ImGui::SliderFloat3("dir_light_1_dir", glm::value_ptr(m_dir_lights[1].dir), -1.0f, 1.0f);
            ImGui::ColorEdit3("dir_light_1_color", glm::value_ptr(m_dir_lights[1].color));
        }

        glm::vec3 m_diffuse   = glm::vec3(0.78f, 0.71f, 0.85f);
        glm::vec3 m_specular  = glm::vec3(1.00f, 1.00f, 1.00f);
        glm::vec3 m_ambient   = glm::vec3(0.00f, 0.00f, 0.00f);
        float     m_shininess = 128.0f;

        std::array<DirLight, 2> m_dir_lights = {{
            DirLight{{+0.5, +0.5, +1.0}, {1.0, 0.9, 0.9}},
            DirLight{{-1.0, +0.0, +0.0}, {0.2, 0.2, 0.5}},
        }};

    private:
        /// \brief Boolean indicating whether a diffuse texture is set by another class or not.
        bool m_is_textured = false;

        // -------------------------------------------------------------------------------------------------------------
        // Handles managed by this class.
        // -------------------------------------------------------------------------------------------------------------

        bgfx::UniformHandle u_params;
        bgfx::UniformHandle u_tex_transform;
        bgfx::UniformHandle s_tex_diffuse;

        /// \brief Texture handle that will be used when no diffuse texture is set.
        bgfx::TextureHandle m_tex_diffuse_alt = BGFX_INVALID_HANDLE;

        // -------------------------------------------------------------------------------------------------------------
        // Handles not managed by this class.
        // -------------------------------------------------------------------------------------------------------------

        /// \brief Texture handle for diffuse color, which should be set and managed by another class.
        bgfx::TextureHandle* m_tex_diffuse = nullptr;
    };
} // namespace bigger

#endif // BIGGER_BLINNPHONG_MATERIAL_HPP
