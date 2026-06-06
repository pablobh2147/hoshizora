#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace hzr {

Camera::Camera(float fov, float near, float far) : m_vertical_fov(fov), m_near(near), m_far(far) {}

void Camera::CalculateProjection() {
    m_projection = glm::perspective(glm::radians(m_vertical_fov), float(m_viewport_width) / float(m_viewport_height), m_near, m_far);
    m_inverse_projection = glm::inverse(m_projection);
}

void Camera::CalculateView() {
    m_view = glm::lookAt(m_position, m_position + m_direction, glm::vec3(0, 1, 0));
    m_inverse_view = glm::inverse(m_view);
}

void Camera::CalculateRays() {
    m_rays.resize(m_viewport_width * m_viewport_height);

    for (int y = 0; y < m_viewport_height; y++) {
        for (int x = 0; x < m_viewport_width; x++) {
            glm::vec2 coord = glm::vec2(float(x) / float(m_viewport_width), float(y) / float(m_viewport_height));
            coord = coord * 2.0f - 1.0f;

            glm::vec4 ray = m_inverse_projection * glm::vec4(coord.x, coord.y, 1.0f, 1.0f);
            glm::vec3 ray_direction = glm::vec3(m_inverse_view * glm::vec4(glm::normalize(glm::vec3(ray) / ray.w), 0));
            m_rays[x + y * m_viewport_width] = ray_direction;
        }
    }
}

void Camera::Resize(uint32_t width, uint32_t height) {
    m_viewport_width = width;
    m_viewport_height = height;

    CalculateProjection();
    CalculateView();
    CalculateRays();
}

glm::vec3 Camera::GetRayDirection(uint32_t x, uint32_t y) const {
    return m_rays[x + y * m_viewport_width];
}

}  // namespace hzr