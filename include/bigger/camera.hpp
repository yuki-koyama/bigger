#ifndef camera_hpp
#define camera_hpp

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace bigger
{
    struct Camera
    {
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up;

        glm::mat4 getViewMatrix() const
        {
            return glm::lookAt(position, target, up);
        }
    };
}

#endif /* camera_hpp */
