#pragma once

#include "math/Matrix.hpp"
#include "math/Vector.hpp"

namespace hzr {

class Camera {
   public:
    Camera(float fov, float near, float far);

    const Matrix4f& GetProjection() const { return m_projection; }
    const Matrix4f& GetView() const { return m_view; }

    void CalculateProjection(float aspect);
    void CalculateView(Vector3f position, Vector3f target, Vector3f up);

   private:
    Matrix4f m_projection = {1};
    Matrix4f m_view = {1};

    float m_fov = 0.0F;
    float m_near = 0.0F;
    float m_far = 0.0F;
};

}  // namespace hzr
