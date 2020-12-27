#ifndef BIGGER_MESH_PRIMITIVE_HPP
#define BIGGER_MESH_PRIMITIVE_HPP

#include <string>
#include <bigger/primitive.hpp>

namespace bigger
{
    class MeshPrimitive : public Primitive
    {
    public:
        
        MeshPrimitive(const std::string& obj_path);
    };
}

#endif // BIGGER_MESH_PRIMITIVE_HPP
