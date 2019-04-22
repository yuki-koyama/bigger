#ifndef plane_primitive_hpp
#define plane_primitive_hpp

#include <bigger/primitive.hpp>

namespace bigger
{
    class PlanePrimitive : public Primitive
    {
    public:

        PlanePrimitive()
        {
            m_vertices =
            {
                { { +0.5f, +0.0f, -0.5f }, { 0.0f, +1.0f, 0.0f } },
                { { -0.5f, +0.0f, -0.5f }, { 0.0f, +1.0f, 0.0f } },
                { { -0.5f, +0.0f, +0.5f }, { 0.0f, +1.0f, 0.0f } },
                { { +0.5f, +0.0f, +0.5f }, { 0.0f, +1.0f, 0.0f } },
            };

            m_triangle_list =
            {
                0, 1, 2,
                0, 2, 3,
            };

            Primitive::initializePrimitive();
        }
    };
}

#endif /* plane_primitive_hpp */
