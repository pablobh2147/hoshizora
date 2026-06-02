#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float vertical_fov, float near, float far) : vertical_fov(vertical_fov), near(near), far(far) {
    direction = glm::vec3(0, 0, -1);
    position = glm::vec3(0, 0, 3);
}

void Camera::recalculateProjection() {
    projection = glm::perspective(glm::radians(vertical_fov), float(viewport_width) / float(viewport_height), near, far);
    inverse_projection = glm::inverse(projection);
}

void Camera::recalculateView() {
    view = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
    inverse_view = glm::inverse(view);
}

void Camera::recalculateRays() {
    rays.resize(viewport_width * viewport_height);

    for (int y = 0; y < viewport_height; y++) {
        for (int x = 0; x < viewport_width; x++) {
            glm::vec2 coord = glm::vec2(float(x) / float(viewport_width), float(y) / float(viewport_height));
            coord = coord * 2.0f - 1.0f;

            glm::vec4 ray = inverse_projection * glm::vec4(coord.x, coord.y, 1.0f, 1.0f);
            glm::vec3 ray_direction = glm::vec3(inverse_view * glm::vec4(glm::normalize(glm::vec3(ray) / ray.w), 0));
            rays[x + y * viewport_width] = ray_direction;
        }
    }
}

void Camera::resize(unsigned int width, unsigned int height) {
    this->viewport_width = width;
    this->viewport_height = height;

    recalculateProjection();
    recalculateView();
    recalculateRays();
}

const glm::mat4& Camera::getProjection() const {
    return projection;
}

const glm::mat4& Camera::getInverseProjection() const {
    return inverse_projection;
}

const glm::mat4& Camera::getView() const {
    return view;
}

const glm::mat4& Camera::getInverseView() const {
    return inverse_view;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

const glm::vec3& Camera::getDirection() const {
    return direction;
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
    recalculateView();
    recalculateRays();
}

void Camera::setDirection(const glm::vec3& direction) {
    this->direction = direction;
    recalculateView();
    recalculateRays();
}

void Camera::move(const glm::vec3& delta) {
    position += delta;
    recalculateView();
    recalculateRays();
}

const std::vector<glm::vec3>& Camera::getRays() const {
    return rays;
}