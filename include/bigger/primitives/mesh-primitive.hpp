#ifndef BIGGER_MESH_PRIMITIVE_HPP
#define BIGGER_MESH_PRIMITIVE_HPP

#include <bigger/primitive.hpp>
#include <string>

namespace bigger
{
    class MeshPrimitive : public Primitive
    {
    public:
        MeshPrimitive(const std::string& obj_path);
    };
} // namespace bigger

#endif // BIGGER_MESH_PRIMITIVE_HPP
