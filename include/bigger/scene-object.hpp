#ifndef scene_object_hpp
#define scene_object_hpp

#include <memory>
#include <glm/glm.hpp>

namespace bigger
{
    class Material;

    class SceneObject
    {
    public:

        SceneObject(std::shared_ptr<Material> material = nullptr) : m_material(material) {}

        virtual void update() {}
        virtual void draw(const glm::mat4& parent_transform_matrix = glm::mat4(1.0f)) {}

        glm::mat4 m_rotate_matrix = glm::mat4(1.0f);
        glm::mat4 m_scale_matrix = glm::mat4(1.0f);
        glm::mat4 m_translate_matrix = glm::mat4(1.0f);

        glm::mat4 getTransform() const
        {
            return m_translate_matrix * m_rotate_matrix * m_scale_matrix;
        }

        bool m_is_active = true;
        bool m_is_visible = true;

        std::shared_ptr<Material> m_material;
    };
}

#endif /* scene_object_hpp */
