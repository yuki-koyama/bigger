#ifndef mesh_primitive_h
#define mesh_primitive_h

#include <string>
#include <bigger/primitives/abstract-primitive.hpp>

namespace bigger
{
    class MeshPrimitive : public AbstractPrimitive
    {
    public:

        MeshPrimitive(const std::string& obj_path);

    private:

        void prepareBuffers() override;
    };
}

#endif /* mesh_primitive_h */
