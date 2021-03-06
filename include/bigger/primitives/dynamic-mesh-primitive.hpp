#ifndef BIGGER_DYNAMIC_MESH_PRIMITIVE_HPP
#define BIGGER_DYNAMIC_MESH_PRIMITIVE_HPP

#include <bigger/primitive.hpp>

namespace bigger
{
    class DynamicMeshPrimitive : public Primitive
    {
    public:
        DynamicMeshPrimitive(const std::vector<PositionNormalUvVertex>& vertex_data,
                             const std::vector<uint16_t>&               triangle_list);

        virtual void initializePrimitive();
        virtual void submitPrimitive(bgfx::ProgramHandle program, bool preserve_state = false) const;
        virtual void destroyPrimitive();

        void updateVertexData(const std::vector<PositionNormalUvVertex>& vertex_data);

    private:
        bgfx::DynamicVertexBufferHandle m_dynamic_vertex_buffer_handle;
    };
} // namespace bigger

#endif // BIGGER_DYNAMIC_MESH_PRIMITIVE_HPP
