#include <array>
#include <memory>
#include <bigger/app.hpp>
#include <bigger/scene-object.hpp>
#include <bigger/materials/blinnphong-material.hpp>
#include <bigger/primitives/cube-primitive.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class CubeObject;

class CubesApp final : public bigger::App
{
public:

    CubesApp();

    void initialize(int argc, char** argv) override;
    void onReset() override;

    void updateApp() override;
    void releaseSharedResources() override;

    // State variables
    int m_massive_level;

    // Const values
    const int m_max_massive_level = 8;

private:

    // Shared resources
    std::shared_ptr<bigger::BlinnPhongMaterial> m_cube_material;
    std::shared_ptr<bigger::CubePrimitive> m_cube_primitive;
};

class CubeObject final : public bigger::SceneObject
{
public:

    CubeObject(const CubesApp* app,
               const int x,
               const int y,
               std::shared_ptr<bigger::BlinnPhongMaterial> material,
               std::shared_ptr<bigger::CubePrimitive> cube) :
    bigger::SceneObject(material),
    m_x(x),
    m_y(y),
    m_app(app),
    m_cube(cube)
    {
        assert(app != nullptr);
        assert(material != nullptr);
        assert(cube != nullptr);
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
        const glm::mat4 transform_matrix = parent_transform_matrix * getTransform();
        bgfx::setTransform(glm::value_ptr(transform_matrix));

        m_material->submitUniforms();
        m_cube->submitPrimitive(m_material->m_program);
    }

    const int m_x;
    const int m_y;

private:

    // Pointer to the app
    const CubesApp* m_app;

    // Assigned resources
    std::shared_ptr<bigger::CubePrimitive> m_cube;
};

CubesApp::CubesApp()
{
    getCamera().m_position = glm::vec3(0.0f, 0.0f, - 5.0f);

    m_massive_level = 2;
}

void CubesApp::initialize(int argc, char** argv)
{
    // Register and apply BGFX configuration settings
    reset(BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X8);

    // Instantiate shared resources
    m_cube_material = std::make_shared<bigger::BlinnPhongMaterial>();
    m_cube_primitive = std::make_shared<bigger::CubePrimitive>();

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

void CubesApp::updateApp()
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
        m_cube_material->drawImgui();
        ImGui::Separator();
        ImGui::SliderInt("massive_level", &m_massive_level, 1, m_max_massive_level);
    }
    ImGui::End();
}

void CubesApp::releaseSharedResources()
{
    // Release shared resources
    m_cube_material = nullptr;
    m_cube_primitive = nullptr;
}

int main(int argc, char** argv)
{
    CubesApp app;
    return app.runApp(argc, argv);
}
