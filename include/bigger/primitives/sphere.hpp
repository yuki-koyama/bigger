#ifndef sphere_hpp
#define sphere_hpp

#include "abstract-primitive.hpp"

class Sphere : public AbstractPrimitive
{
public:

    Sphere() : AbstractPrimitive() {}

private:

    void prepareBuffers() override
    {
        constexpr double pi = glm::pi<double>();
        constexpr int latitude_resolution  = 20;
        constexpr int longitude_resolution = 30;

        m_vertices.resize(latitude_resolution * longitude_resolution * 6);
        m_triangle_list.resize(latitude_resolution * longitude_resolution * 6);

        for (int i = 0; i < longitude_resolution; ++ i)
        {
            const double theta_xy_1 = 2.0 * static_cast<double>(i + 0) * pi / static_cast<double>(longitude_resolution);
            const double theta_xy_2 = 2.0 * static_cast<double>(i + 1) * pi / static_cast<double>(longitude_resolution);
            const double x_1        = std::cos(theta_xy_1);
            const double x_2        = std::cos(theta_xy_2);
            const double y_1        = std::sin(theta_xy_1);
            const double y_2        = std::sin(theta_xy_2);

            for (int j = 0; j < latitude_resolution; ++ j)
            {
                const double theta_z_1 = static_cast<double>(j + 0) * pi / static_cast<double>(latitude_resolution);
                const double theta_z_2 = static_cast<double>(j + 1) * pi / static_cast<double>(latitude_resolution);
                const double cos_1 = std::cos(theta_z_1);
                const double cos_2 = std::cos(theta_z_2);
                const double sin_1 = std::sin(theta_z_1);
                const double sin_2 = std::sin(theta_z_2);

                const int offset = i * latitude_resolution * 6 + j * 6;

                const glm::vec3 vertex_0 = { sin_2 * x_1, sin_2 * y_1, cos_2 };
                const glm::vec3 vertex_1 = { sin_2 * x_2, sin_2 * y_2, cos_2 };
                const glm::vec3 vertex_2 = { sin_1 * x_2, sin_1 * y_2, cos_1 };
                const glm::vec3 vertex_3 = { sin_2 * x_1, sin_2 * y_1, cos_2 };
                const glm::vec3 vertex_4 = { sin_1 * x_2, sin_1 * y_2, cos_1 };
                const glm::vec3 vertex_5 = { sin_1 * x_1, sin_1 * y_1, cos_1 };

                m_vertices[offset + 0] = { vertex_0, vertex_0 };
                m_vertices[offset + 1] = { vertex_1, vertex_1 };
                m_vertices[offset + 2] = { vertex_2, vertex_2 };
                m_vertices[offset + 3] = { vertex_3, vertex_3 };
                m_vertices[offset + 4] = { vertex_4, vertex_4 };
                m_vertices[offset + 5] = { vertex_5, vertex_5 };
            }
        }

        for (int i = 0; i < m_triangle_list.size(); ++ i)
        {
            m_triangle_list[i] = static_cast<uint16_t>(i);
        }
    }
};

#endif /* sphere_hpp */
