#include <array>
#include <cmath>
#include <memory>
#include <bigger/app.hpp>
#include <bigger/material.hpp>
#include <bigger/scene-object.hpp>
#include <bigger/primitives/mesh-primitive.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class MeshMaterial;
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
    std::shared_ptr<MeshMaterial> m_mesh_material;
    std::shared_ptr<bigger::MeshPrimitive> m_mesh_primitive;
};

class MeshMaterial final : public bigger::Material
{
public:

    MeshMaterial() : bigger::Material("blinnphong")
    {
        m_handle = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, 3);
    }

    ~MeshMaterial()
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
    std::shared_ptr<bigger::MeshPrimitive> m_primitive;
};

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

void MeshApp::updateApp()
{
    // Display ImGui components
    ImGui::Begin("Config");
    {
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
    return app.run(argc, argv);
}
