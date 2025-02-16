#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace GameEngine
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity, Scene*);
        Entity(const Entity&) = default;

        template<typename T, typename ... Args>
        T& addComponent(Args&& ... args)
        {
            CORE_ASSERT(!hasComponent<T>(), "Component already exists");
            return scene->registry.emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent()
        {
            CORE_ASSERT(hasComnponent<T>(), "Component doesn't exist");
            return scene->registry.get<T>(entity);
        }

        template<typename T>
        bool hasComponent()
        {
            return scene->registry.all_of<T>(entity);
        }

        template<typename T>
        void removeComponent()
        {
            CORE_ASSERT(hasComponent<T>(), "Component doesn't exist");
            scene->registry.remove<T>(entity);
        }

        operator uint32_t() const { return (uint32_t)entity; }
        operator entt::entity() const { return entity; }
        operator bool() const { return entity != entt::null; }

    private:
        entt::entity entity = entt::null;
        Scene* scene = nullptr;
    };
}