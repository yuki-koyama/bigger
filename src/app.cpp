#include <bigger/app.hpp>
#include <bigger/scene-object.hpp>
#include <stdexcept>
#include <rand-util.hpp>

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

    // Call the application-specific update method
    updateApp();
}

int bigger::App::shutdown()
{
    // Release the scene objects
    m_scene_objects.clear();

    // Release the application-specific shared resources
    releaseSharedResources();

    return 0;
}
