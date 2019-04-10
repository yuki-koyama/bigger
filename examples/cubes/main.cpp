#include <cmath>
#include <memory>
#include <bigger/bigger.hpp>
#include <bigger/scene-object.hpp>
#include <bigger/primitives/cube-primitive.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <rand-util.hpp>

class CubeMaterial;
class CubeObject;

class CubesApp : public bigger::Application
{
public:

    CubesApp();

    void initialize(int argc, char** argv) override;
    void onReset() override;
    void update(float dt) override;
    int shutdown() override;

    // State variables
    float m_time;
    int m_massive_level;

    // Const variables
    const int m_max_massive_level = 8;

private:

    void addSceneObject(std::shared_ptr<CubeObject> cube_object, const std::string& name = "");

    // Shared resources
    std::shared_ptr<CubeMaterial> m_cube_material;
    std::shared_ptr<bigger::CubePrimitive> m_cube_primitive;
};

class Material
{
public:
    bgfx::ProgramHandle m_program;
};

class CubeMaterial : public Material
{
public:
    CubeMaterial()
    {
        const std::string shader_dir_path = bigger::getShaderDirectoryPath(bgfx::getRendererType());
        const std::string vs_path = shader_dir_path + "/" + "vs_blinnphong.bin";
        const std::string fs_path = shader_dir_path + "/" + "fs_blinnphong.bin";

        m_program = bigg::loadProgram(vs_path.c_str(), fs_path.c_str());
    }

    ~CubeMaterial()
    {
        bgfx::destroy(m_program);
    }
};

class CubeObject : public bigger::SceneObject
{
public:

    CubeObject(const CubesApp* app,
               const int x,
               const int y,
               std::shared_ptr<CubeMaterial> material = nullptr,
               std::shared_ptr<bigger::CubePrimitive> cube = nullptr) :
    m_x(x),
    m_y(y),
    m_app(app)
    {
        if (material == nullptr)
        {
            m_material = std::make_shared<CubeMaterial>();
        }
        else
        {
            m_material = material;
        }

        if (cube == nullptr)
        {
            m_cube = std::make_shared<bigger::CubePrimitive>();
            m_cube->initializePrimitive();
        }
        else
        {
            m_cube = cube;
        }
    }

    ~CubeObject()
    {
        m_cube = nullptr;
    }

    void update() override
    {
        // Update transform
        const float t = m_app->m_time;
        m_rotate_matrix = glm::rotate(t, glm::vec3(m_x, m_y, 1.0f));

        // Update visibility
        m_is_visible = std::max(std::abs(m_x), std::abs(m_y)) <= m_app->m_massive_level;
    }

    void draw(const glm::mat4& parent_transform_matrix = glm::mat4(1.0f)) override
    {
        const glm::mat4 transform_matrix = parent_transform_matrix * m_translate_matrix * m_rotate_matrix * m_scale_matrix;
        bgfx::setTransform(glm::value_ptr(transform_matrix));
        m_cube->submitPrimitive(m_material->m_program);
    }

    const int m_x;
    const int m_y;

private:

    // Pointer to the app
    const CubesApp* m_app;

    // Assigned resources
    std::shared_ptr<CubeMaterial> m_material;
    std::shared_ptr<bigger::CubePrimitive> m_cube;
};

CubesApp::CubesApp()
{
    m_time = 0.0f;
    m_massive_level = 2;
}

void CubesApp::initialize(int argc, char** argv)
{
    // Register and apply BGFX configuration settings
    reset(BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X8);

    // Instantiate shared resources
    m_cube_material = std::make_shared<CubeMaterial>();
    m_cube_primitive = std::make_shared<bigger::CubePrimitive>();
    m_cube_primitive->initializePrimitive();

    // Instantiate scene objects
    for (int x = - m_max_massive_level; x <= m_max_massive_level; ++ x)
    {
        for (int y = - m_max_massive_level; y <= m_max_massive_level; ++ y)
        {
            auto cube_object = std::make_shared<CubeObject>(this, x, y, m_cube_material, m_cube_primitive);

            cube_object->m_translate_matrix = glm::translate(glm::vec3(x, y, 0.0f));
            cube_object->m_scale_matrix = glm::scale(glm::vec3(std::pow(3.0f, - 0.5f)));

            addSceneObject(cube_object);
        }
    }
}

void CubesApp::onReset()
{
    constexpr uint32_t bg_color = 0x303030ff;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, bg_color, 1.0f, 0);
}

void CubesApp::update(float dt)
{
    // Update state variables
    m_time += dt;

    // Display ImGui components
    ImGui::Begin("Config");
    {
        ImGui::Text("time: %.2f", m_time);
        ImGui::Text("fps: %.2f", 1.0f / dt);
        ImGui::Separator();
        ImGui::SliderInt("massive_level", &m_massive_level, 1, m_max_massive_level);
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

int CubesApp::shutdown()
{
    // Clear scene objects
    m_scene_objects.clear();

    // Release shared resources
    m_cube_material = nullptr;
    m_cube_primitive = nullptr;

    return 0;
}

void CubesApp::addSceneObject(std::shared_ptr<CubeObject> cube_object, const std::string& name)
{
    if (name.empty())
    {
        const std::string random_name = randutil::GenRandomString();
        m_scene_objects[random_name] = cube_object;
    }
    else
    {
        const bool has_the_same_name_object = m_scene_objects.find(name) != m_scene_objects.end();
        if (has_the_same_name_object)
        {
            throw std::runtime_error("");
        }

        m_scene_objects[name] = cube_object;
    }
}

int main(int argc, char** argv)
{
    CubesApp app;
    return app.run(argc, argv);
}
