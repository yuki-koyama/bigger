#include <cmath>
#include <memory>
#include <bigger/bigger.hpp>
#include <bigger/material.hpp>
#include <bigger/scene-object.hpp>
#include <bigger/primitives/mesh-primitive.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class MeshMaterial;
class MeshObject;

class MeshApp final : public bigger::Application
{
public:

    MeshApp();

    void initialize(int argc, char** argv) override;
    void onReset() override;
    void update(float dt) override;
    int shutdown() override;

    // State variables
    float m_time;

private:

    // Shared resources
    std::shared_ptr<MeshMaterial> m_mesh_material;
    std::shared_ptr<bigger::MeshPrimitive> m_mesh_primitive;
};

class MeshMaterial final : public bigger::Material
{
public:
    MeshMaterial() : bigger::Material("blinnphong") {}
};

class MeshObject final : public bigger::SceneObject
{
public:

    MeshObject(const MeshApp* app,
               std::shared_ptr<MeshMaterial> material = nullptr,
               std::shared_ptr<bigger::MeshPrimitive> mesh = nullptr) :
    m_app(app)
    {
        if (material == nullptr)
        {
            m_material = std::make_shared<MeshMaterial>();
        }
        else
        {
            m_material = material;
        }

        if (mesh == nullptr)
        {
            throw std::runtime_error("");
        }

        m_primitive = mesh;
    }

    ~MeshObject()
    {
        m_primitive = nullptr;
    }

    void update() override
    {
        // Update transform
        const float t = m_app->m_time;
        m_rotate_matrix = glm::rotate(t, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void draw(const glm::mat4& parent_transform_matrix = glm::mat4(1.0f)) override
    {
        const glm::mat4 transform_matrix = parent_transform_matrix * getTransform();
        bgfx::setTransform(glm::value_ptr(transform_matrix));
        m_primitive->submitPrimitive(m_material->m_program);
    }

private:

    // Pointer to the app
    const MeshApp* m_app;

    // Assigned resources
    std::shared_ptr<MeshMaterial> m_material;
    std::shared_ptr<bigger::MeshPrimitive> m_primitive;
};

MeshApp::MeshApp()
{
    m_time = 0.0f;
}

void MeshApp::initialize(int argc, char** argv)
{
    // Register and apply BGFX configuration settings
    reset(BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X8);

    // Instantiate shared resources
    m_mesh_material = std::make_shared<MeshMaterial>();

    const std::string obj_path = "models/teapot.obj";
    m_mesh_primitive = std::make_shared<bigger::MeshPrimitive>(obj_path);
    m_mesh_primitive->initializePrimitive();

    // Instantiate scene objects
    auto mesh_object = std::make_shared<MeshObject>(this, m_mesh_material, m_mesh_primitive);

    addSceneObject(mesh_object);
}

void MeshApp::onReset()
{
    constexpr uint32_t bg_color = 0x303030ff;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, bg_color, 1.0f, 0);
}

void MeshApp::update(float dt)
{
    // Update state variables
    m_time += dt;

    // Display ImGui components
    ImGui::Begin("Config");
    {
        ImGui::Text("time: %.2f", m_time);
        ImGui::Text("fps: %.2f", 1.0f / dt);
        ImGui::Separator();
        ImGui::SliderFloat3("camera.position", glm::value_ptr(getCamera().position), - 10.0f, 10.0f);
    }
    ImGui::End();

    // Prepare drawing
    setViewProj();
    setRect();
    bgfx::touch(0);

    // Update scene objects
    for (auto key_value : m_scene_objects)
    {
        if (key_value.second->m_is_active)
        {
            key_value.second->update();
        }
    }

    // Draw scene objects
    for (auto key_value : m_scene_objects)
    {
        if (key_value.second->m_is_active && key_value.second->m_is_visible)
        {
            key_value.second->draw();
        }
    }
}

int MeshApp::shutdown()
{
    // Clear scene objects
    m_scene_objects.clear();

    // Release shared resources
    m_mesh_material = nullptr;
    m_mesh_primitive = nullptr;

    return 0;
}

int main(int argc, char** argv)
{
    MeshApp app;
    return app.run(argc, argv);
}
