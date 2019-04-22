#include <bigger/primitives/mesh-primitive.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

bigger::MeshPrimitive::MeshPrimitive(const std::string& obj_path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    const bool return_value = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_path.c_str());

    if (!warn.empty()) { std::cerr << warn << std::endl; }
    if (!err.empty()) { std::cerr << err << std::endl; }
    if (!return_value) { throw std::runtime_error(""); }

    if (attrib.vertices.empty() || attrib.normals.empty()) { throw std::runtime_error(""); }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            m_vertices.push_back(
            {
                {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                },
                {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                }
            });
            m_triangle_list.push_back(uint16_t(m_triangle_list.size()));
        }
    }

    Primitive::initializePrimitive();
}
