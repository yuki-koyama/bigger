#include <array>
#include <cmath>
#include <memory>
#include <bigger/app.hpp>
#include <bigger/scene-object.hpp>
#include <bigger/materials/blinnphong-material.hpp>
#include <bigger/primitives/mesh-primitive.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class MeshObject;

class MeshApp final : public bigger::App
{
public:

    MeshApp()
    {
        getCamera().m_target = glm::vec3(0.0f, 0.3f, 0.0f);
    }

    void initialize(int argc, char** argv) override;
    void onReset() override;

    void updateApp() override;
    void releaseSharedResources() override;

private:

    // Shared resources
    std::shared_ptr<bigger::BlinnPhongMaterial> m_mesh_material;
    std::shared_ptr<bigger::MeshPrimitive> m_mesh_primitive;
};

class MeshObject final : public bigger::SceneObject
{
public:

    MeshObject(const MeshApp* app,
               std::shared_ptr<bigger::BlinnPhongMaterial> material,
               std::shared_ptr<bigger::MeshPrimitive> mesh) :
    bigger::SceneObject(material),
    m_app(app),
    m_primitive(mesh)
    {
        assert(app != nullptr);
        assert(material != nullptr);
        assert(mesh != nullptr);
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

        m_material->submitUniforms();
        m_primitive->submitPrimitive(m_material->m_program);
    }

private:

    // Pointer to the app
    const MeshApp* m_app;

    // Assigned resources
    std::shared_ptr<bigger::MeshPrimitive> m_primitive;
};

void MeshApp::initialize(int argc, char** argv)
{
    // Register and apply BGFX configuration settings
    reset(BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X8);

    // Instantiate shared resources
    m_mesh_material = std::make_shared<bigger::BlinnPhongMaterial>();

    const std::string obj_path = "models/teapot.obj";
    m_mesh_primitive = std::make_shared<bigger::MeshPrimitive>(obj_path);

    // Instantiate scene objects
    auto mesh_object = std::make_shared<MeshObject>(this, m_mesh_material, m_mesh_primitive);

    addSceneObject(mesh_object);
}

void MeshApp::onReset()
{
    constexpr uint32_t bg_color = 0x303030ff;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, bg_color, 1.0f, 0);
}

void MeshApp::updateApp()
{
    // Display ImGui components
    ImGui::Begin("Config");
    {
        ImGui::Text("frame: %d", m_frame);
        ImGui::Text("time: %.2f", m_time);
        ImGui::Text("fps: %.2f", 1.0f / m_last_dt);
        ImGui::Separator();
        ImGui::SliderFloat3("camera.position", glm::value_ptr(getCamera().m_position), - 10.0f, 10.0f);
        ImGui::SliderFloat("camera.fov", &(getCamera().m_fov), 10.0f, 120.0f);
        ImGui::Separator();
        m_mesh_material->drawImgui();
    }
    ImGui::End();
}

void MeshApp::releaseSharedResources()
{
    // Release shared resources
    m_mesh_material = nullptr;
    m_mesh_primitive = nullptr;
}

int main(int argc, char** argv)
{
    MeshApp app;
    return app.runApp(argc, argv);
}
