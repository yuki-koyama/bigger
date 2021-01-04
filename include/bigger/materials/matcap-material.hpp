#ifndef BIGGER_MATCAP_MATERIAL_HPP
#define BIGGER_MATCAP_MATERIAL_HPP

#include <array>
#include <bgfx/bgfx.h>
#include <bigger/material.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

namespace bigger
{
    class MatcapMaterial final : public Material
    {
    public:
        MatcapMaterial() : bigger::Material("matcap")
        {
            s_tex_matcap = bgfx::createUniform("s_tex_matcap", bgfx::UniformType::Sampler);

            constexpr bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA32F;
            constexpr uint64_t                  flags  = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;

            m_tex_matcap_alt = bgfx::createTexture2D(1, 1, false, 1, format, flags, nullptr);

#if BX_PLATFORM_OSX
            // This is an ad-hoc hotfix to avoid the Metal API Validation assertion; see
            // https://github.com/yuki-koyama/bigger/issues/13
            bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
#endif
        }

        ~MatcapMaterial()
        {
            bgfx::destroy(s_tex_matcap);
            bgfx::destroy(m_tex_matcap_alt);
        }

        void setTexMatcap(bgfx::TextureHandle* tex_diffuse)
        {
            assert(isValid(*tex_diffuse));

            m_tex_matcap  = tex_diffuse;
            m_is_textured = true;
        }

        void submitUniforms() override
        {
            // Set the diffuse color
            if (m_is_textured)
            {
                bgfx::setTexture(0, s_tex_matcap, *m_tex_matcap);
            }
            else
            {
                // Create and set a 1x1 small diffuse color texture
                static float        color[4] = {1.0f, 0.0f, 1.0f, 1.0f};
                const bgfx::Memory* mem      = bgfx::makeRef(color, sizeof(float) * 4);

                // Update the texture with the current color
                bgfx::updateTexture2D(m_tex_matcap_alt, 0, 0, 0, 0, 1, 1, mem);
                bgfx::setTexture(0, s_tex_matcap, m_tex_matcap_alt);
            }
        }

        void drawImgui() override { ImGui::Text("Material Setting"); }

    private:
        /// \brief Boolean indicating whether a diffuse texture is set by another class or not.
        bool m_is_textured = false;

        // -------------------------------------------------------------------------------------------------------------
        // Handles managed by this class.
        // -------------------------------------------------------------------------------------------------------------

        bgfx::UniformHandle s_tex_matcap;

        /// \brief Texture handle that will be used when no diffuse texture is set.
        bgfx::TextureHandle m_tex_matcap_alt = BGFX_INVALID_HANDLE;

        // -------------------------------------------------------------------------------------------------------------
        // Handles not managed by this class.
        // -------------------------------------------------------------------------------------------------------------

        /// \brief Texture handle for diffuse color, which should be set and managed by another class.
        bgfx::TextureHandle* m_tex_matcap = nullptr;
    };
} // namespace bigger

#endif // BIGGER_MATCAP_MATERIAL_HPP
