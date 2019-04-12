#ifndef mesh_primitive_h
#define mesh_primitive_h

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

#endif /* mesh_primitive_h */
