#include <bigger/camera.hpp>
#include <imgui.h>

void bigger::Camera::drawImgui()
{
    ImGui::Text("Camera Setting");
    ImGui::SliderFloat3("camera.position", glm::value_ptr(m_position), -10.0f, 10.0f);
    ImGui::SliderFloat("camera.fov", &m_fov, 10.0f, 120.0f);
}
