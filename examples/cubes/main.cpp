#include <bigger/bigger.hpp>
#include <bigger/camera.hpp>
#include <bigger/primitives/cube.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class CubeApp : public bigg::Application
{
public:

    CubeApp();
    void initialize(int argc, char** argv) override;
    void onReset() override;
    void update(float dt) override;
    int shutdown() override;

private:

    float getAspect() const { return float(getWidth()) / float(getHeight()); }

    bgfx::ProgramHandle m_program;
    bigger::Cube m_cube;

    bigger::Camera m_camera;
    float m_time;
};

CubeApp::CubeApp()
{
    m_camera.position = { 0.0f, 0.0f, - 5.0f };
    m_camera.target   = { 0.0f, 0.0f, 0.0f };
    m_camera.up       = { 0.0f, 1.0f, 0.0f };
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
    bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
}

void CubeApp::update(float dt)
{
    m_time += dt;

    const glm::mat4 view_matrix = m_camera.getViewMatrix();
    const glm::mat4 proj_matrix = glm::perspective(glm::radians(60.0f), getAspect(), 0.1f, 100.0f);

    bgfx::setViewTransform(0, glm::value_ptr(view_matrix), glm::value_ptr(proj_matrix));
    bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
    bgfx::touch(0);

    static int n = 4;

    ImGui::Begin("Config");
    {
        ImGui::SliderInt("n", &n, 1, 8);
        ImGui::SliderFloat3("m_camera.position", glm::value_ptr(m_camera.position), - 5.0f, 5.0f);
    }
    ImGui::End();

    for (int x = - n; x <= n; ++ x)
    {
        for (int y = - n; y <= n; ++ y)
        {
            glm::mat4 model_matrix(1.0f);
            model_matrix = glm::translate(model_matrix, glm::vec3(x, y, 0.0f));
            model_matrix = glm::rotate(model_matrix, m_time, glm::vec3(x, y, 1.0f));
            model_matrix = glm::scale(model_matrix, glm::vec3(2.0f / n));
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
