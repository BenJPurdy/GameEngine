#pragma once
#include "Scene.h"
#include "entt/entt.hpp"

namespace GameEngine
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity, Scene*);
        Entity(const Entity&) = default;

        

        //template <typename T, typename... Args>
        //T& addComponent(Args&&... args)
        //{
        //    CORE_ASSERT(!hasComponent<T>(), "Entity has component");
        //    return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
        //}

        template<typename T>
        T& addComponent(T&& arg)
        {
            CORE_ASSERT(!hasComponent<T>(), "Entity has component");
            return scene->registry.emplace<T>(entityHandle, arg);
        }

        template<typename T>
        T& addComponent()
        {
            CORE_ASSERT(!hasComponent<T>(), "Entity has component");
            return scene->registry.emplace<T>(entityHandle);
        }

        template <typename T>
        T& getComponent()
        {
            CORE_ASSERT(hasComponent<T>(), "Entity does not have component");
            return scene->registry.get<T>(entityHandle);
        }

        template <typename T>
        bool hasComponent()
        {
            return scene->registry.all_of<T>(entityHandle);
        }

        template <typename T>
        void removeComponent()
        {
            CORE_ASSERT(hasComponent<T>(), "Entity does not have component");
            scene->registry.remove<T>(entityHandle);
        }

        operator uint32_t() const { return (uint32_t)entityHandle; }
        operator entt::entity() const { return entityHandle; }
        operator bool() const { return entityHandle != entt::null; }

    private:
        entt::entity entityHandle = entt::null;
        Scene* scene = nullptr;
    };
}