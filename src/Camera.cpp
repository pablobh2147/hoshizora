#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace hzr {

Camera::Camera(float fov, float near, float far) : m_fov(fov), m_near(near), m_far(far) {}

void Camera::CalculateProjection(float aspect) {
    m_projection = glm::perspective(glm::radians(m_fov), aspect, m_near, m_far);
}

void Camera::CalculateView(Vector3f position, Vector3f target, Vector3f up) {
    m_view = glm::lookAt(position, target, up);
}

}  // namespace hzr