#include <format>
#include <iostream>
#include <string>

#include "Camera.hpp"
#include "ImageBuffer.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"

using namespace rtx;

void ConfigureScene(Scene& scene) noexcept {
    scene.background_color = glm::vec3(0.6f, 0.7f, 0.9f);

    Material material_1;
    material_1.albedo = glm::vec3(1.0f, 0.0f, 1.0f);
    material_1.roughness = 0.04f;

    scene.materials.push_back(material_1);
    scene.spheres.push_back(Sphere(glm::vec3(0, 0.2f, 0), 1.2f, 0));

    Material material_2;
    material_2.albedo = glm::vec3(0.0f, 0.0f, 1.0f);
    material_2.roughness = 0.6f;

    scene.materials.push_back(material_2);
    scene.spheres.push_back(Sphere(glm::vec3(0.0f, -201.0, 0.0f), 200.0f, 1));

    Material material_3;
    material_3.albedo = glm::vec3(1.0f, 1.0f, 0.0f);
    material_3.roughness = 0.05f;
    material_3.emissive = glm::vec3(245 / 255.0f, 167 / 255.0f, 66 / 255.0f);
    material_3.emissive_strength = 40.0f;

    scene.materials.push_back(material_3);
    scene.spheres.push_back(Sphere(glm::vec3(100.0f, 0.5f, -300), 100, 2));
}

int main() {
    constexpr uint32_t OUTPUT_WIDTH = 512;
    constexpr uint32_t OUTPUT_HEIGHT = 512;

    ImageBuffer buffer = ImageBuffer(OUTPUT_WIDTH, OUTPUT_HEIGHT);
    Renderer renderer;

    Camera camera(45.0F, 0.01F, 100.0F);
    camera.SetPosition(glm::vec3(15, 10, 15));
    camera.SetDirection(glm::vec3(-1, -0.6, -1));
    camera.Resize(OUTPUT_WIDTH, OUTPUT_HEIGHT);

    Scene scene;
    ConfigureScene(scene);

    bool render_animation = true;

    RenderOptions options = {
        .samples = 128,
        .max_bounces = 5,
    };

    if (render_animation) {
        constexpr uint32_t TOTAL_FRAMES = 24;
        constexpr glm::vec3 MOVE_STEP = glm::vec3(0.25F, 0, 0.25F);

        for (uint32_t i = 1; i <= TOTAL_FRAMES; i++) {
            std::cout << "Rendering frame " << i << " of " << TOTAL_FRAMES << std::endl;
            camera.Move(MOVE_STEP);

            renderer.RenderFrame(scene, camera, buffer, options);

            std::string filename = std::format("output/image-{}.png", i);
            WriteImageToDisk(buffer, filename.c_str());
            std::cout << "Rendered frame " << i << " of " << TOTAL_FRAMES << std::endl;
        }
    } else {
        renderer.RenderFrame(scene, camera, buffer, options);
        WriteImageToDisk(buffer, "output/image.png");
    }

    return 0;
}