#include <cmath>
#include <memory>
#include <unordered_map>
#include <bigger/bigger.hpp>
#include <bigger/primitives/cube.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <rand-util.hpp>

class SceneObject
{
public:
    glm::mat4 m_rotate_matrix = glm::mat4(1.0f);
    glm::mat4 m_scale_matrix = glm::mat4(1.0f);
    glm::mat4 m_translate_matrix = glm::mat4(1.0f);
};

class CubeObject : public SceneObject
{
public:
    CubeObject(const int x,
               const int y,
               const bgfx::ProgramHandle program,
               std::shared_ptr<bigger::Cube> cube = nullptr) :
    m_x(x),
    m_y(y)
    {
        m_program = program;

        if (cube == nullptr)
        {
            m_cube = std::make_shared<bigger::Cube>();
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

    void update(const float time)
    {
        m_rotate_matrix = glm::rotate(time, glm::vec3(m_x, m_y, 1.0f));
    }

    void draw(const glm::mat4& parent_transform_matrix = glm::mat4(1.0f))
    {
        const glm::mat4 transform_matrix = parent_transform_matrix * m_translate_matrix
         * m_rotate_matrix * m_scale_matrix;
        bgfx::setTransform(glm::value_ptr(transform_matrix));
        m_cube->submitPrimitive(m_program);
    }

    const int m_x;
    const int m_y;

private:
    // Assigned resources
    bgfx::ProgramHandle m_program;
    std::shared_ptr<bigger::Cube> m_cube;
};

class CubeApp : public bigger::Application
{
public:

    CubeApp();

    void initialize(int argc, char** argv) override;
    void onReset() override;
    void update(float dt) override;
    int shutdown() override;

    void addSceneObject(std::shared_ptr<CubeObject> cube_object, const std::string& name = "")
    {
        if (name.empty())
        {
            const std::string random_name = randutil::GenRandomString();
            m_cube_objects[random_name] = cube_object;
        }
        else
        {
            const bool has_the_same_name_object = m_cube_objects.find(name) != m_cube_objects.end();
            if (has_the_same_name_object)
            {
                throw std::runtime_error("");
            }

            m_cube_objects[name] = cube_object;
        }
    }

private:
    // Shared resources
    std::vector<bgfx::ProgramHandle> m_programs;

    // Scene objects
    std::unordered_map<std::string, std::shared_ptr<CubeObject>> m_cube_objects;

    // State variables
    float m_time;
    int m_massive_level;
};

CubeApp::CubeApp()
{
    m_massive_level = 2;
}

void CubeApp::initialize(int argc, char** argv)
{
    // Build a shader program
    const std::string shader_dir_path = bigger::getShaderDirectoryPath(bgfx::getRendererType());
    const std::string vs_path = shader_dir_path + "/" + "vs_blinnphong.bin";
    const std::string fs_path = shader_dir_path + "/" + "fs_blinnphong.bin";
    bgfx::ProgramHandle program = bigg::loadProgram(vs_path.c_str(), fs_path.c_str());

    // Instantiate scene objects
    for (int x = - m_massive_level; x <= m_massive_level; ++ x)
    {
        for (int y = - m_massive_level; y <= m_massive_level; ++ y)
        {
            auto cube_object = std::make_shared<CubeObject>(x, y, program);

            cube_object->m_translate_matrix = glm::translate(glm::vec3(x, y, 0.0f));
            cube_object->m_scale_matrix = glm::scale(glm::vec3(std::pow(3.0f, - 0.5f)));

            addSceneObject(cube_object);
        }
    }

    // Register shared resources
    m_programs.push_back(program);
}

void CubeApp::onReset()
{
    constexpr uint32_t bg_color = 0x303030ff;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, bg_color, 1.0f, 0);
}

void CubeApp::update(float dt)
{
    m_time += dt;

    ImGui::Begin("Config");
    {
#if 0 // Dynamic cube instantiation is not supported currently
        ImGui::SliderInt("massive_level", &m_massive_level, 1, 8);
#endif
        ImGui::SliderFloat3("camera.position", glm::value_ptr(getCamera().position), - 5.0f, 5.0f);
    }
    ImGui::End();

    setViewProj();
    setRect();

    bgfx::touch(0);

    for (auto key_value : m_cube_objects)
    {
        key_value.second->update(m_time);
    }

    for (auto key_value : m_cube_objects)
    {
        key_value.second->draw();
    }
}

int CubeApp::shutdown()
{
    for (auto program : m_programs)
    {
        bgfx::destroy(program);
    }
    m_cube_objects.clear();

    return 0;
}

int main(int argc, char** argv)
{
    CubeApp app;
    return app.run(argc, argv);
}
