#include <bigger/primitives/mesh-primitive.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

bigger::MeshPrimitive::MeshPrimitive(const std::string& obj_path) :
bigger::AbstractPrimitive()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    bool return_value = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_path.c_str());

    if (!warn.empty()) { std::cerr << warn << std::endl; }
    if (!err.empty()) { std::cerr << err << std::endl; }
    if (!return_value) { throw std::runtime_error(""); }

    // TODO
}

void bigger::MeshPrimitive::prepareBuffers()
{
    // TODO
}
