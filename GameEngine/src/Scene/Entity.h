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
        

        template <typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            CORE_ASSERT(!hasComponent<T>(), "Entity has component");
            T& comp = scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
            scene->onComponentAdded<T>(*this, comp);

            return comp;
        }

        //template<typename T>
        //T& addComponent(T&& arg)
        //{
        //    CORE_ASSERT(!hasComponent<T>(), "Entity has component");
        //    return scene->registry.emplace<T>(entityHandle, arg);
        //}
        //
        //template<typename T>
        //T& addComponent()
        //{
        //    CORE_ASSERT(!hasComponent<T>(), "Entity has component");
        //    return scene->registry.emplace<T>(entityHandle);
        //}

        template <typename T>
        T& getComponent()
        {
            CORE_ASSERT(hasComponent<T>(), "Entity does not have component");
            return scene->registry.get<T>(entityHandle);
        }

        template <typename T>
        T& getComponent() const
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
        bool hasComponent() const
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

        bool operator==(const Entity& other) const
        {
            return entityHandle == other.entityHandle && scene == other.scene;
        }

        bool operator!= (const Entity& other) const { return !(*this == other); }

        template <typename T>
        void addComponentFromScript()
        {
            if (hasComponent<T>())
                return;
            auto& c = addComponent<T>();
        }

        entt::entity getEntt() { return entityHandle; }
        Scene* getScene() { return scene; }
        const Scene* getConstScene() { return scene; }

        UUID getUUID() const { return getComponent<IDComponent>().id; }
        UUID getSceneID() const { return scene->sceneID; }

    private:
        entt::entity entityHandle = entt::null;
        Scene* scene = nullptr;
        void* entityData;

        friend class Scene;
        friend class SceneSerialiser;
    };
}