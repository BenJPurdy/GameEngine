#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <string>

#include "Core/UUID.h"
#include "SceneCamera.h"

namespace GameEngine
{

    struct IDComponent
    {
        UUID id = 0;
        IDComponent(UUID& comp) : id(comp) {}

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
    };
    struct TagComponent
    {
        std::string tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& t) : tag(t) {}
    };

    struct TransformComponent
    {
        glm::vec3 transform = { 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& t) : transform(t) {}

        glm::mat4 getTransform() const
        {
            glm::mat4 rot = glm::toMat4(glm::quat(rotation));
            return glm::translate(glm::mat4(1.0f), transform) * rot *
                glm::scale(glm::mat4(1.0f), scale);
        }
    };

    struct CameraComponent
    {
        SceneCamera camera;
        bool primary = true;
        bool fixedAspect = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(ProjectionType t)
        {
            camera.setProjectionType(t);
        }

        operator SceneCamera& () { return camera; }
        operator const SceneCamera& () const { return camera; }
    };

    struct SpriteRenderComponent
    {
        glm::vec4 colour =  {1.0f, 1.0f, 0.0f, 1.0f};

        SpriteRenderComponent() = default;
        SpriteRenderComponent(const SpriteRenderComponent&) = default;
        SpriteRenderComponent(const glm::vec4& c) : colour(c) {}
    };
}