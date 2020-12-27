#ifndef BIGGER_CAMERA_HPP
#define BIGGER_CAMERA_HPP

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace bigger
{
    class Camera
    {
    public:
        glm::vec3 m_position = glm::vec3(1.0f, 1.0f, -2.0f);
        glm::vec3 m_target   = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_up       = glm::vec3(0.0f, 1.0f, 0.0f);

        float m_fov       = 60.0f;
        float m_near_clip = 0.1f;
        float m_far_clip  = 100.0f;

        glm::mat4 getViewMatrix() const { return glm::lookAt(m_position, m_target, m_up); }
    };
} // namespace bigger

#endif // BIGGER_CAMERA_HPP
