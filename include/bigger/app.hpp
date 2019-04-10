#ifndef app_hpp
#define app_hpp

#include <memory>
#include <string>
#include <unordered_map>
#include <bigg.hpp>
#include <bigger/camera.hpp>

namespace bigger
{
    class SceneObject;

    class App : public bigg::Application
    {
    public:

        App() : bigg::Application("", 1280, 720)
        {
            m_camera.position = { 1.0f, 1.0f, - 2.0f };
            m_camera.target   = { 0.0f, 0.0f, 0.0f };
            m_camera.up       = { 0.0f, 1.0f, 0.0f };
        }

        void setRect()
        {
            bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
        }

        void setViewProj()
        {
            const glm::mat4 view_matrix = m_camera.getViewMatrix();
            const glm::mat4 proj_matrix = glm::perspective(glm::radians(60.0f), getAspect(), 0.1f, 100.0f);

            bgfx::setViewTransform(0, glm::value_ptr(view_matrix), glm::value_ptr(proj_matrix));
        }

        float getAspect() const
        {
            return float(getWidth()) / float(getHeight());
        }

        const Camera& getCamera() const { return m_camera; }
        Camera& getCamera() { return m_camera; }

        std::unordered_map<std::string, std::shared_ptr<SceneObject>> m_scene_objects;

        void addSceneObject(std::shared_ptr<SceneObject> scene_object, const std::string& name = "");

    private:

        Camera m_camera;
    };
}

#endif /* app_hpp */
