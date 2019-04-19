#include <bigger/app.hpp>
#include <bigger/material.hpp>
#include <bigger/scene-object.hpp>
#include <bigger/screen-shot-callback.hpp>
#include <stdexcept>
#include <rand-util.hpp>

int bigger::App::runApp(int argc, char** argv, bgfx::RendererType::Enum type)
{
    static ScreenShotCallback callback;
    return run(argc, argv, type, BGFX_PCI_ID_NONE, 0, &callback);
}

void bigger::App::addSceneObject(std::shared_ptr<SceneObject> scene_object, const std::string& name)
{
    if (name.empty())
    {
        const std::string random_name = randutil::GenRandomString();
        m_scene_objects[random_name] = scene_object;
    }
    else
    {
        const bool has_the_same_name_object = m_scene_objects.find(name) != m_scene_objects.end();
        if (has_the_same_name_object)
        {
            throw std::runtime_error("");
        }

        m_scene_objects[name] = scene_object;
    }
}

void bigger::App::update(float dt)
{
    // Update state variables
    m_last_dt = dt;
    m_time += dt;
    ++ m_frame;

    // Call the application-specific update method
    updateApp();

    // Update scene objects
    for (auto key_value : m_scene_objects)
    {
        if (key_value.second->m_is_active)
        {
            key_value.second->update();
        }
    }

    // Prepare drawing
    setViewProj();
    setRect();
    bgfx::touch(0);

    // Draw scene objects
    for (auto key_value : m_scene_objects)
    {
        if (key_value.second->m_is_active && key_value.second->m_is_visible)
        {
            key_value.second->draw();
        }
    }
}

int bigger::App::shutdown()
{
    // Release the scene objects
    m_scene_objects.clear();

    // Release the application-specific shared resources
    releaseSharedResources();

    return 0;
}
