#ifndef dynamic_mesh_primitive_hpp
#define dynamic_mesh_primitive_hpp

#include <bigger/primitive.hpp>

namespace bigger
{
    class DynamicMeshPrimitive : public Primitive
    {
    public:

        DynamicMeshPrimitive(const std::vector<PositionNormalVertex>& vertex_data,
                             const std::vector<uint16_t>& triangle_list);

        virtual void initializePrimitive();
        virtual void submitPrimitive(bgfx::ProgramHandle program, bool preserve_state = false) const;
        virtual void destroyPrimitive();

        void updateVertexData(const std::vector<PositionNormalVertex>& vertex_data);

    private:

        bgfx::DynamicVertexBufferHandle m_dynamic_vertex_buffer_handle;
    };
}

#endif /* dynamic_mesh_primitive_hpp */
