#pragma once

#include <glm/glm.hpp>

namespace rtx {

class Sphere;

struct Raycast {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct RaycastResult {
    bool hit = false;
    glm::vec3 hitpoint = glm::vec3(0, 0, 0);
    glm::vec3 normal = glm::vec3(0, 0, 0);
    const Sphere* body = nullptr;
};

}  // namespace rtx
