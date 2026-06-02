#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace rtx {

class Camera {
   public:
    Camera(float fov, float near, float far);

    void Resize(uint32_t width, uint32_t height);

    const glm::mat4& GetProjection() const { return m_projection; }
    const glm::mat4& GetInverseProjection() const { return m_inverse_projection; }
    const glm::mat4& GetView() const { return m_view; }
    const glm::mat4& GetInverseView() const { return m_inverse_view; }

    const glm::vec3& GetPosition() const { return m_position; }
    const glm::vec3& GetDirection() const { return m_direction; }

    void SetPosition(const glm::vec3& position) {
        m_position = position;
        CalculateView();
        CalculateRays();
    }

    void SetDirection(const glm::vec3& direction) {
        m_direction = direction;
        CalculateView();
        CalculateRays();
    }

    void Move(const glm::vec3& delta) {
        m_position += delta;
        CalculateView();
        CalculateRays();
    }

    glm::vec3 GetRayDirection(uint32_t x, uint32_t y) const;

   private:
    void CalculateProjection();
    void CalculateView();
    void CalculateRays();

    glm::mat4 m_projection = {1};
    glm::mat4 m_inverse_projection = {1};
    glm::mat4 m_view = {1};
    glm::mat4 m_inverse_view = {1};

    float m_vertical_fov = 0.0F;
    float m_near = 0.0F;
    float m_far = 0.0F;

    glm::vec3 m_position = {0, 0, 0};
    glm::vec3 m_direction = {0, 0, 0};

    std::vector<glm::vec3> m_rays;

    uint32_t m_viewport_width = 0;
    uint32_t m_viewport_height = 0;
};

}  // namespace rtx
