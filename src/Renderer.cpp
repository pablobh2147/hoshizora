#include "Renderer.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <execution>
#include <iostream>

namespace rtx {

void Renderer::RenderFrame(const Scene& scene, const Camera& camera, ImageBuffer& buffer, const RenderOptions& options) {
    ResetSamples(buffer.GetWidth(), buffer.GetHeight());
    for (uint32_t i = 0; i < options.samples; i++) {
        std::cout << "Sample " << i << " of " << options.samples << std::endl;
        RenderSample(scene, camera, buffer);
    }
}

void Renderer::RenderSample(const Scene& scene, const Camera& camera, ImageBuffer& buffer) {
    this->current_camera = &camera;
    this->current_scene = &scene;
    this->width = buffer.GetWidth();
    this->height = buffer.GetHeight();

    std::for_each(std::execution::par, vertical_iter.begin(),
                  vertical_iter.end(), [this, &buffer](uint32_t y) {
                      for (uint32_t x = 0; x < width; x++) {
                          Color color = ProcessFragment(x, y);
                          accumulated_colors[x + y * width] += color;

                          Color accum = accumulated_colors[x + y * width];
                          accum /= accumulated_samples;
                          accum = glm::clamp(accum, 0.0f, 1.0f);

                          buffer.SetPixel(x, y, calculateColorFromRGBF(accum.r, accum.g, accum.b));
                      }
                  });

    accumulated_samples++;
}

void Renderer::ResetSamples(uint32_t width, uint32_t height) {
    accumulated_samples = 1;

    if (this->width != width || this->height != height) {
        delete[] accumulated_colors;
        accumulated_colors = new Color[width * height];
    }

    std::memset(accumulated_colors, 0x00, width * height * sizeof(Color));

    vertical_iter.resize(height);
    for (uint32_t i = 0; i < height; i++) {
        vertical_iter[i] = i;
    }
}

static uint32_t PCG_Hash(uint32_t input) {
    uint32_t state = input * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float randomFloat(uint32_t& seed) {
    seed = PCG_Hash(seed);
    return float(seed) / float(UINT32_MAX);
}

glm::vec3 randomUnitSphere(uint32_t& seed) {
    return glm::normalize(
        glm::vec3(
            randomFloat(seed) * 2.0f - 1.0f,
            randomFloat(seed) * 2.0f - 1.0f,
            randomFloat(seed) * 2.0f - 1.0f));
}

Color Renderer::ProcessFragment(uint32_t x, uint32_t y) {
    Raycast ray = {
        .origin = current_camera->getPosition(),
        .direction = current_camera->getRays()[x + y * width],
    };

    glm::vec3 light(0.0F);
    glm::vec3 contribution(1.0F);

    uint32_t seed = x + y * width;
    seed *= accumulated_samples;

    uint32_t bounces = 5;
    for (uint32_t i = 0; i < bounces; i++) {
        seed += i;

        RaycastResult result = TraceRay(ray);
        if (!result.hit) {
            light += current_scene->background_color * contribution;
            break;
        }

        const Material& material =
            current_scene->materials[result.body->material_idx];

        contribution *= material.albedo;
        light += material.getEmmision();

        ray.origin = result.hitpoint + result.normal * 0.0001F;
        ray.direction = glm::normalize(result.normal + randomUnitSphere(seed));
    }

    return glm::vec4(light, 1.0f);
}

RaycastResult Renderer::TraceRay(const Raycast& ray) {
    const Sphere* closest_sphere = nullptr;
    float closest_t = std::numeric_limits<float>::max();
    for (const Sphere& sphere : current_scene->spheres) {
        glm::vec3 origin = ray.origin - sphere.position;

        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(origin, ray.direction);
        float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) {
            continue;
        }

        float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        if (t0 > 0.0f && t0 < closest_t) {
            closest_t = t0;
            closest_sphere = &sphere;
        }
    }

    if (closest_sphere == nullptr) {
        return RaycastResult();
    }

    return ClosestHit(ray, closest_t, closest_sphere);
}

RaycastResult Renderer::ClosestHit(const Raycast& ray, float hit_distance, const Sphere* sphere) {
    RaycastResult result;
    result.hit = true;
    result.body = sphere;

    glm::vec3 origin = ray.origin - sphere->position;
    result.hitpoint = origin + ray.direction * hit_distance;
    result.normal = glm::normalize(result.hitpoint);

    result.hitpoint += sphere->position;

    return result;
}

}  // namespace rtx