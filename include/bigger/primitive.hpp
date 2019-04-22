#ifndef primitive_hpp
#define primitive_hpp

#include <cassert>
#include <vector>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

namespace bigger
{
    struct PositionNormalVertex
    {
        glm::vec3 position;
        glm::vec3 normal;

        static bgfx::VertexDecl getVertexDecl()
        {
            bgfx::VertexDecl vertex_decl;
            vertex_decl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();
            return vertex_decl;
        }
    };

    class Primitive
    {
    public:

        Primitive() : m_is_initialized(false) {}
        virtual ~Primitive() { destroyPrimitive(); }

        virtual void submitPrimitive(bgfx::ProgramHandle program, bool preserve_state = false) const
        {
            assert(m_is_initialized);

            bgfx::setVertexBuffer(0, m_vertex_buffer_handle);
            bgfx::setIndexBuffer(m_index_buffer_handle);

            bgfx::submit(0, program, bgfx::ViewMode::Default, preserve_state);
        }

    protected:

        std::vector<PositionNormalVertex> m_vertices;
        std::vector<uint16_t> m_triangle_list;

        bool m_is_initialized;

        bgfx::VertexBufferHandle m_vertex_buffer_handle;
        bgfx::IndexBufferHandle m_index_buffer_handle;

        virtual void initializePrimitive()
        {
            assert(!m_vertices.empty());
            assert(!m_triangle_list.empty());

            const bgfx::VertexDecl vertex_decl = PositionNormalVertex::getVertexDecl();
            m_vertex_buffer_handle = bgfx::createVertexBuffer(bgfx::makeRef(m_vertices.data(), sizeof(PositionNormalVertex) * m_vertices.size()), vertex_decl);
            m_index_buffer_handle = bgfx::createIndexBuffer(bgfx::makeRef(m_triangle_list.data(), sizeof(uint16_t) * m_triangle_list.size()));

            m_is_initialized = true;
        }

    private:

        virtual void destroyPrimitive()
        {
            assert(m_is_initialized);

            bgfx::destroy(m_vertex_buffer_handle);
            bgfx::destroy(m_index_buffer_handle);
        }
    };
}

#endif /* primitive_hpp */
