#ifndef abstract_primitive_hpp
#define abstract_primitive_hpp

#include <cassert>
#include <vector>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

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

class AbstractPrimitive
{
public:

    AbstractPrimitive()
    : m_is_initialized(false)
    {
    }

    ~AbstractPrimitive()
    {
        if (m_is_initialized)
        {
            bgfx::destroy(m_vertex_buffer_handle);
        }
    }

    void initializePrimitive()
    {
        prepareBuffers();

        const bgfx::VertexDecl vertex_decl = PositionNormalVertex::getVertexDecl();
        m_vertex_buffer_handle = bgfx::createVertexBuffer(bgfx::makeRef(m_vertices.data(), sizeof(PositionNormalVertex) * m_vertices.size()), vertex_decl);
        m_index_buffer_handle = bgfx::createIndexBuffer(bgfx::makeRef(m_triangle_list.data(), sizeof(uint16_t) * m_triangle_list.size()));

        m_is_initialized = true;
    }

    void submitPrimitive(bgfx::ProgramHandle program) const
    {
        assert(m_is_initialized);

        bgfx::setVertexBuffer(0, m_vertex_buffer_handle);
        bgfx::setIndexBuffer(m_index_buffer_handle);

        bgfx::submit(0, program);
    }

protected:

    virtual void prepareBuffers() = 0;

    std::vector<PositionNormalVertex> m_vertices;
    std::vector<uint16_t> m_triangle_list;

private:

    bool m_is_initialized;

    bgfx::VertexBufferHandle m_vertex_buffer_handle;
    bgfx::IndexBufferHandle m_index_buffer_handle;
};

#endif /* abstract_primitive_hpp */
