#pragma once

#include "Core/Timestep.h"
#include "entt/entt.hpp"
#include "Core/UUID.h"

#include "Components.h"
#include <typeindex>

#include "Renderer/Camera/EditorCamera.h"


namespace GameEngine
{
    class Entity;

    class Scene
    {
    public:
        Scene(const std::string& name = "UntitledScene", bool isEditor = false);
        ~Scene();

        static Ref<Scene> copy(Ref<Scene>);
        static void Scene::copyTo(Ref<Scene>, Ref<Scene>);

        Entity createEntity(const std::string& name = std::string());
        Entity createEntityWithUUID(UUID, const std::string& = "");

        Entity duplicateEntity(Entity);

        entt::registry& getRegistry() { return registry; }

        //void onUpdate(Timestep);
        void onUpdateEditor(Timestep, EditorCamera&);
        void onUpdateRuntime(Timestep);

        void destoryEntity(Entity);
        void onViewportResize(uint32_t, uint32_t);

        void onRuntimeStart();
        void onRuntimeStop();

        template<typename T>
        void registerComponentHandler(std::function<void(Entity, T&)> hand)
        { 
            componentHandlers[typeid(T)] = [hand](Entity entity)
                {
                    hand(entity, entity.getComponent<T>());
                };       
        }

        template<typename T>
        void onComponentAdded(Entity e, T& c)
        {
            auto it = componentHandlers.find(typeid(T));
            if (it != componentHandlers.end())
            {
                it->second(e);
            }
            else
            {
                LOG_INFO("Component of type {0} added to entity ID {1}", typeid(T).name(), (uint32_t)e);
            }
        }

        const std::string& getName() { return name; }
        void setName(const std::string& n) { name = n; }

        template <typename T>
        void copyComponentIfExists(entt::entity dst, entt::registry& dstReg, entt::entity src)
        {
            if (registry.any_of<T>(src))
            {
                auto& srcComp = registry.get<T>(src);
                dstReg.emplace_or_replace<T>(dst, srcComp);
            }
        }
        template <typename T>
        static void copyComponent(entt::registry& dst, entt::registry& src,
            const std::unordered_map<UUID, entt::entity>& enttMap)
        {
            auto srcEntities = src.view<T>();
            for (auto e : srcEntities)
            {
                entt::entity dstEnitity = enttMap.at(src.get<IDComponent>(src).id);
                auto& srcComp = src.get<T>(e);
                auto dstComp = dst.emplace_or_replace<T>(dstEnitity, srcComp);
            }
        }


        uint32_t getViewportWidth() { return viewportWidth; }
        uint32_t getViewpotHeight() { return viewportHeight; }

    private:


        entt::registry registry;
        friend class Entity;
        //friend class SceneHierarchyPanel;

        std::unordered_map<std::type_index, std::function<void(Entity)>> componentHandlers;

        uint32_t viewportWidth = 0, viewportHeight = 0;

        std::string name;
        bool isEditorScene = false;

    };
}