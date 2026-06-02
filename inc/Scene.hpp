#pragma once

#include <vector>

#include "Material.hpp"
#include "Sphere.hpp"

namespace rtx {

struct Scene {
    glm::vec3 background_color = glm::vec3(0.0f, 0.0f, 0.0f);
    std::vector<Sphere> spheres;
    std::vector<Material> materials;
};

}  // namespace rtx
