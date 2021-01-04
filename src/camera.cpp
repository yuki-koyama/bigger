#include <bigger/camera.hpp>
#include <imgui.h>

void bigger::Camera::drawImgui()
{
    ImGui::Text("Camera Setting");
    ImGui::SliderFloat3("Position", glm::value_ptr(m_position), -5.0f, 5.0f);
    ImGui::SliderFloat3("Target", glm::value_ptr(m_target), -2.0f, 2.0f);
    ImGui::SliderFloat("FoV (Degree)", &m_fov, 10.0f, 120.0f);
}
