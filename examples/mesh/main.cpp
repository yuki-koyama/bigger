#include <cmath>
#include <memory>
#include <unordered_map>
#include <bigger/bigger.hpp>
#include <bigger/scene-object.hpp>
#include <bigger/primitives/mesh-primitive.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <rand-util.hpp>

class MeshMaterial;
class MeshObject;

class MeshApp : public bigger::Application
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

    void addSceneObject(std::shared_ptr<MeshObject> cube_object, const std::string& name = "");

    // Shared resources
    std::shared_ptr<MeshMaterial> m_mesh_material;
    std::shared_ptr<bigger::MeshPrimitive> m_mesh_primitive;

    // Scene objects
    std::unordered_map<std::string, std::shared_ptr<MeshObject>> m_objects;
};

class Material
{
public:
    bgfx::ProgramHandle m_program;
};

class MeshMaterial : public Material
{
public:
    MeshMaterial()
    {
        const std::string shader_dir_path = bigger::getShaderDirectoryPath(bgfx::getRendererType());
        const std::string vs_path = shader_dir_path + "/" + "vs_blinnphong.bin";
        const std::string fs_path = shader_dir_path + "/" + "fs_blinnphong.bin";

        m_program = bigg::loadProgram(vs_path.c_str(), fs_path.c_str());
    }

    ~MeshMaterial()
    {
        bgfx::destroy(m_program);
    }
};

class MeshObject : public bigger::SceneObject
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
        const glm::mat4 transform_matrix = parent_transform_matrix * m_translate_matrix * m_rotate_matrix * m_scale_matrix;
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

    const std::string obj_path = "";
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
    for (auto key_value : m_objects)
    {
        if (key_value.second->m_is_active)
        {
            key_value.second->update();
        }
    }

    // Draw scene objects
    for (auto key_value : m_objects)
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
    m_objects.clear();

    // Release shared resources
    m_mesh_material = nullptr;
    m_mesh_primitive = nullptr;

    return 0;
}

void MeshApp::addSceneObject(std::shared_ptr<MeshObject> cube_object, const std::string& name)
{
    if (name.empty())
    {
        const std::string random_name = randutil::GenRandomString();
        m_objects[random_name] = cube_object;
    }
    else
    {
        const bool has_the_same_name_object = m_objects.find(name) != m_objects.end();
        if (has_the_same_name_object)
        {
            throw std::runtime_error("");
        }

        m_objects[name] = cube_object;
    }
}

int main(int argc, char** argv)
{
    MeshApp app;
    return app.run(argc, argv);
}
