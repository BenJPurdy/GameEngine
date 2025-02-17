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
        glm::mat4 transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& t) : transform(t) {}

        operator glm::mat4& () { return transform; }
        operator const glm::mat4& () const { return transform; }

        void translate(glm::vec3 v) { transform = glm::translate(transform, v); }
        void scale(glm::vec3 s) { transform = glm::scale(transform, s); }
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
        glm::vec4 colour {1.0};

        SpriteRenderComponent() = default;
        SpriteRenderComponent(const SpriteRenderComponent&) = default;
        SpriteRenderComponent(const glm::vec4& c) : colour(c) {}
    };
}