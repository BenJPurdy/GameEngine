#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <string>

#include "Core/UUID.h"
#include "SceneCamera.h"

#include "box2d/box2d.h"

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
        bool mod = true;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& t) : transform(t) {}

        glm::mat4 getTransform() const
        {
            glm::mat4 rot = glm::toMat4(glm::quat(rotation));
            return glm::translate(glm::mat4(1.0f), transform) * rot *
                glm::scale(glm::mat4(1.0f), scale);
        }
        //getters/setters and set mod to true
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

    struct ScriptComponent
    {
        std::string script = (512, "");
        std::string onStart = (512, "");
        std::string onUpdate = (512, "");
        std::string onCollisionEnter = (512, "");
        std::string onCollisionExit = (512, "");
        std::string onDestory = (512, "");

        void* onStartPtr = nullptr;
        void* onUpdatePtr = nullptr;
        void* onCollisionEnterPtr = nullptr;
        void* onCollisionExitPtr = nullptr;
        void* onDestoryPtr = nullptr;


        void call(void*)
        {

        }
        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
        ScriptComponent(const std::string s) : onUpdate(s) {}
    };

    struct CircleRenderComponent
    {
        glm::vec4 colour{ 1.0f };
        float radius = 0.5f;
        float thickness = 1.0f;
        float fade = 0.005f;

        CircleRenderComponent() = default;
        CircleRenderComponent(const CircleRenderComponent&) = default;
        CircleRenderComponent(const glm::vec4 & c) : colour(c) {}
    };

    enum PhysicsProperties : uint16_t
    {
        PhysProps_None = 0,
        PhysProps_Static = 0x1,         //BIT(1)
        PhysProps_Dynamic = 0x2,        //BIT(2)
        PhysProps_Kinematic = 0x4,      //BIT(3)
        PhysProps_FixedRotation = 0x8   //BIT(4)
    };

    struct Body
    {
        float mass = 1.0f;

    };

    

    struct Rigidbody2dComponent
    {
        b2BodyId id = b2_nullBodyId; //it will move
        uint16_t properties = PhysicsProperties::PhysProps_None;
        Body body;
        bool has(uint16_t c) { return (properties & c) == c; };
        void set(uint16_t c) { properties |= c; }
        uint16_t getBodyType() { return properties & 0b00000111; }
    };

    struct Collider2d
    {
        b2ShapeId id = b2_nullShapeId; //it won't move
        glm::vec2 offset = { 0.0f, 0.0f };
        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.5f;
        void setDefaults()
        {
            this->offset = { 0.0f, 0.0f };
            this->density = 1.0f;
            this->friction = 0.5f;
            this->restitution = 0.5f;
        }
        bool isSensor = false;
    };

    struct BoxCollider2dComponent : Collider2d
    {
        glm::vec2 extents = { 1.0f, 1.0f };
        void setDefaults()
        {
            Collider2d::setDefaults();
            this->extents = { 1.0f, 1.0f };
        }
    };

    struct CircleCollider2dComponent : Collider2d
    {
        float radius = 1.0f;
        void setDefaults()
        {
            Collider2d::setDefaults();
            this->radius = 1.0f;
        }
    };
}