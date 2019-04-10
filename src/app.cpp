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

int bigger::App::shutdown()
{
    m_scene_objects.clear();
    releaseSharedResources();

    return 0;
}
