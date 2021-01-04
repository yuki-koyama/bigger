#include <array>
#include <bigger/app.hpp>
#include <bigger/materials/matcap-material.hpp>
#include <bigger/primitives/mesh-primitive.hpp>
#include <bigger/scene-object.hpp>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>

class MeshObject;

class MatcapApp final : public bigger::App
{
public:
    MatcapApp() { getCamera().m_target = glm::vec3(0.0f, 0.1f, 0.0f); }

    void initialize(int argc, char** argv) override;
    void onReset() override;

    void updateApp() override;
    void releaseSharedResources() override;

private:
    // Shared resources
    std::shared_ptr<bigger::MatcapMaterial> m_mesh_material;
    std::shared_ptr<bigger::MeshPrimitive>  m_mesh_primitive;
    std::array<bgfx::TextureHandle, 4>      m_tex_matcaps;

    // Current state
    int m_matcap_index = 1;

    // Static resources
    static constexpr std::array<const char*, 4> k_matcap_paths = {
        "assets/matcap/ceramic_dark.png",
        "assets/matcap/ceramic_lightbulb.png",
        "assets/matcap/clay_studio.png",
        "assets/matcap/jade.png",
    };
    static constexpr std::array<const char*, 4> k_matcap_names = {
        "Ceramic Dark",
        "Ceramic Lightbulb",
        "Clay Studio",
        "Jade",
    };
};

class MeshObject final : public bigger::SceneObject
{
public:
    MeshObject(const MatcapApp*                       app,
               std::shared_ptr<bigger::Material>      material,
               std::shared_ptr<bigger::MeshPrimitive> mesh)
        : bigger::SceneObject(material), m_app(app), m_primitive(mesh)
    {
        assert(app != nullptr);
        assert(material != nullptr);
        assert(mesh != nullptr);
    }

    void update() override
    {
        // Update transform
        const float t   = m_app->m_time;
        m_rotate_matrix = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void draw(const glm::mat4& parent_transform_matrix = glm::mat4(1.0f)) override
    {
        const glm::mat4 transform_matrix = parent_transform_matrix * getTransform();
        bgfx::setTransform(glm::value_ptr(transform_matrix));

        m_material->submitUniforms();
        m_primitive->submitPrimitive(m_material->m_program);
    }

private:
    // Pointer to the app
    const MatcapApp* m_app;

    // Assigned resources
    std::shared_ptr<bigger::MeshPrimitive> m_primitive;
};

void MatcapApp::initialize(int argc, char** argv)
{
    assert(std::size(m_tex_matcaps) == std::size(k_matcap_paths));
    assert(std::size(m_tex_matcaps) == std::size(k_matcap_names));

    // Register and apply BGFX configuration settings
    reset(BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X8);

    // Instantiate shared resources (matcap textures)
    for (int i = 0; i < std::size(m_tex_matcaps); ++i)
    {
        m_tex_matcaps[i] = bigger::loadTexture(k_matcap_paths[i]);
    }

    // Instantiate shared resources (matcap material)
    m_mesh_material = std::make_shared<bigger::MatcapMaterial>();

    m_mesh_material->setTexMatcap(&(m_tex_matcaps[m_matcap_index]));

    // Instantiate shared resources (mesh)
    const std::string obj_path = "assets/spot.obj";

    m_mesh_primitive = std::make_shared<bigger::MeshPrimitive>(obj_path);

    // Instantiate scene objects
    auto mesh_object = std::make_shared<MeshObject>(this, m_mesh_material, m_mesh_primitive);

    addSceneObject(mesh_object);
}

void MatcapApp::onReset()
{
    constexpr uint32_t bg_color = 0x303030ff;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, bg_color, 1.0f, 0);
}

void MatcapApp::updateApp()
{
    // Display ImGui components
    ImGui::Begin("Config");
    {
        ImGui::Text("frame: %d", m_frame);
        ImGui::Text("time: %.2f", m_time);
        ImGui::Text("fps: %.2f", 1.0f / m_last_dt);
        ImGui::Separator();
        getCamera().drawImgui();
        ImGui::Separator();
        m_mesh_material->drawImgui();
        ImGui::Separator();
        ImGui::Text("App Setting");
        if (ImGui::Combo("MatCap", &m_matcap_index, k_matcap_names.data(), std::size(k_matcap_names)))
        {
            assert(m_matcap_index < std::size(m_tex_matcaps));

            // Update the MatCap texture
            m_mesh_material->setTexMatcap(&(m_tex_matcaps[m_matcap_index]));
        }
    }
    ImGui::End();
}

void MatcapApp::releaseSharedResources()
{
    // Release shared resources
    m_mesh_material  = nullptr;
    m_mesh_primitive = nullptr;

    for (int i = 0; i < std::size(m_tex_matcaps); ++i)
    {
        bgfx::destroy(m_tex_matcaps[i]);
    }
}

int main(int argc, char** argv)
{
    MatcapApp app;
    return app.runApp(argc, argv);
}
