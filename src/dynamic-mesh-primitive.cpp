#include <bigger/primitives/dynamic-mesh-primitive.hpp>

bigger::DynamicMeshPrimitive::DynamicMeshPrimitive(const std::vector<PositionNormalVertex>& vertex_data,
                                                   const std::vector<uint16_t>& triangle_list)
{
    m_vertices = vertex_data;
    m_triangle_list = triangle_list;

    initializePrimitive();
}

void bigger::DynamicMeshPrimitive::initializePrimitive()
{
    assert(!m_vertices.empty());
    assert(!m_triangle_list.empty());

    const bgfx::VertexDecl vertex_decl = PositionNormalVertex::getVertexDecl();
    m_dynamic_vertex_buffer_handle = bgfx::createDynamicVertexBuffer(bgfx::makeRef(m_vertices.data(), sizeof(PositionNormalVertex) * m_vertices.size()), vertex_decl);
    m_index_buffer_handle = bgfx::createIndexBuffer(bgfx::makeRef(m_triangle_list.data(), sizeof(uint16_t) * m_triangle_list.size()));

    m_is_initialized = true;
}

void bigger::DynamicMeshPrimitive::submitPrimitive(bgfx::ProgramHandle program, bool preserve_state) const
{
    assert(m_is_initialized);
    assert(!m_vertices.empty());
    assert(!m_triangle_list.empty());

    bgfx::update(m_dynamic_vertex_buffer_handle, 0, bgfx::makeRef(m_vertices.data(), sizeof(PositionNormalVertex) * m_vertices.size()));

    bgfx::setVertexBuffer(0, m_dynamic_vertex_buffer_handle);
    bgfx::setIndexBuffer(m_index_buffer_handle);

    bgfx::submit(0, program, bgfx::ViewMode::Default, preserve_state);
}

void bigger::DynamicMeshPrimitive::destroyPrimitive()
{
    assert(m_is_initialized);
    assert(!m_vertices.empty());
    assert(!m_triangle_list.empty());

    bgfx::destroy(m_dynamic_vertex_buffer_handle);
    bgfx::destroy(m_index_buffer_handle);
}

void bigger::DynamicMeshPrimitive::updateVertexData(const std::vector<PositionNormalVertex>& vertex_data)
{
    assert(m_vertices.size() == vertex_data.size());

    m_vertices = vertex_data;
}
