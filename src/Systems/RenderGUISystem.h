#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"

#include <glm.hpp>
#include <imgui.h>
#include <imgui_sdl.h>

class RenderGUISystem : public System
{
public:
    RenderGUISystem() = default;

    void Update(const std::unique_ptr<Registry> &registry, const SDL_Rect &camera)
    {
        ImGui::NewFrame();
        if (ImGui::Begin("Spawn enemies"))
        {
            static int posX = 0;
            static int posY = 0;
            static int scaleX = 1;
            static int scaleY = 1;
            static int velX = 0;
            static int velY = 0;
            static int health = 100;
            static float rotation = 0.0;
            static float projAngle = 0.0;
            static float projSpeed = 100;
            static int projRepeat = 100.0;
            static int projDuration = 10;
            const char *sprites[] = {"tank-image", "truck-image"};
            static int selectedSpriteIndex = 0;

            // Section to input enemy sprite texture id
            if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
            }
            ImGui::Spacing();

            // Section to input enemy tranform values
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputInt("position X", &posX);
                ImGui::InputInt("position Y", &posY);
                ImGui::SliderInt("Scale X", &scaleX, 1, 10);
                ImGui::SliderInt("Scale Y", &scaleY, 1, 10);
                ImGui::SliderAngle("Rotation (deg)", &rotation, 0.0, 360.0);
            }
            ImGui::Spacing();

            // Section to input enemy rigid body values
            if (ImGui::CollapsingHeader("Rigid Body", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputInt("Velocity X", &velX);
                ImGui::InputInt("Velocity Y", &velY);
            }
            ImGui::Spacing();

            // Section to input enemy projectile emitter values
            if (ImGui::CollapsingHeader("Projectile Emitter", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::SliderAngle("Angle (deg)", &projAngle, 0.0, 360.0);
                ImGui::InputFloat("Speed (px/sec)", &projSpeed, 1.0, 500.0);
                ImGui::InputInt("Repeat (sec)", &projRepeat);
                ImGui::InputInt("Duration (sec)", &projDuration);
            }
            ImGui::Spacing();

            // Section to input enemy health value
            if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputInt("Health", &health);
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Spawn tank"))
            {
                Entity enemy = registry->CreateEntity();
                enemy.Group("enemies");
                enemy.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
                enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 2);
                enemy.AddComponent<BoxColliderComponent>(25, 18, glm::vec2(5, 5));
                double projVelX = cos(projAngle) * projSpeed; // convert from angle-speed to x-value
                double projVelY = sin(projAngle) * projSpeed; // convert from angle-speed to y-value
                enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), projRepeat * 1000, projDuration * 1000, 10, false);
                enemy.AddComponent<HealthComponent>(health);

                posX = posY = rotation = projAngle = 0;
                scaleX = scaleY = 1;
                projRepeat = projDuration = 100;
                projSpeed = 100;
                health = 100;
            }
        }
        ImGui::End();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowBgAlpha(0.9f);
        if (ImGui::Begin("Map coordinates", nullptr, window_flags))
        {
            ImGui::Text(
                "Mouse position: (%.1f, %.1f)",
                ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y);
        }
        ImGui::End();

        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};

#endif
