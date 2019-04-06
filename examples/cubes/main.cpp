#include <cmath>
#include <bigger/bigger.hpp>
#include <bigger/primitives/cube.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class CubeApp : public bigger::Application
{
public:

    CubeApp();

    void initialize(int argc, char** argv) override;
    void onReset() override;
    void update(float dt) override;
    int shutdown() override;

private:

    bgfx::ProgramHandle m_program;
    bigger::Cube m_cube;

    float m_time;

    int m_massive_level;
};

CubeApp::CubeApp()
{
    m_massive_level = 2;
}

void CubeApp::initialize(int argc, char** argv)
{
    const std::string shader_dir_path = bigger::getShaderDirectoryPath(bgfx::getRendererType());
    const std::string vs_path = shader_dir_path + "/" + "vs_blinnphong.bin";
    const std::string fs_path = shader_dir_path + "/" + "fs_blinnphong.bin";

    m_program = bigg::loadProgram(vs_path.c_str(), fs_path.c_str());

    m_cube.initializePrimitive();
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
        ImGui::SliderInt("massive_level", &m_massive_level, 1, 8);
        ImGui::SliderFloat3("camera.position", glm::value_ptr(getCamera().position), - 5.0f, 5.0f);
    }
    ImGui::End();

    setViewProj();
    setRect();

    bgfx::touch(0);

    for (int x = - m_massive_level; x <= m_massive_level; ++ x)
    {
        for (int y = - m_massive_level; y <= m_massive_level; ++ y)
        {
            glm::mat4 model_matrix(1.0f);
            model_matrix = glm::translate(model_matrix, glm::vec3(x, y, 0.0f));
            model_matrix = glm::rotate(model_matrix, m_time, glm::vec3(x, y, 1.0f));
            model_matrix = glm::scale(model_matrix, glm::vec3(std::pow(3.0f, - 0.5f)));
            bgfx::setTransform(glm::value_ptr(model_matrix));

            m_cube.submitPrimitive(m_program);
        }
    }
}

int CubeApp::shutdown()
{
    bgfx::destroy(m_program);
    m_cube.destroyPrimitive();

    return 0;
}

int main(int argc, char** argv)
{
    CubeApp app;
    return app.run(argc, argv);
}
