#pragma once

#include "Core/Timestep.h"
#include "entt/entt.hpp"
#include "Core/UUID.h"

#include "Components.h"
#include <typeindex>

#include "Renderer/Camera/EditorCamera.h"

#include "Physics/PhysicsBody.h"

#include "Scripting/Scripting.h"
#include "Audio/Audio.h"
#include "Scripting/API/API.h"
#include "Network/Network.h"



namespace GameEngine
{
    class Network;
    class Entity;

    struct CopiedComponent
    {
        std::type_index type;
        std::function<void(Entity&)> pasteFunction;

        CopiedComponent() : type(typeid(void)) {}
    };

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
        void onUpdateServer(uint32_t = 100);

        void destoryEntity(Entity);
        void onViewportResize(uint32_t, uint32_t);

        void onRuntimeStart(bool = false);
        void onRuntimeStop(bool = false);

        void onServerStart();
        void onServerStop();

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

        template<typename... Components>
        auto getAllEntitiesWith()
        {
            return registry.view<Components...>();
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
                entt::entity dstEnitity = enttMap.at(src.get<IDComponent>(e).id);
                auto& srcComp = src.get<T>(e);
                auto dstComp = dst.emplace_or_replace<T>(dstEnitity, srcComp);
            }
        }

        template<typename T>
        void copyComponent(Entity e)
        {
            if (!e.hasComponent<T>()) return;

            auto& comp = e.getComponent<T>();
            copiedComponent.type = typeid(T);
            copiedComponent.pasteFunction = [comp](Entity& tgt) mutable
                {
                    if (!tgt.hasComponent<T>())
                        tgt.addComponent<T>(comp);
                    else
                        tgt.getComponent<T>() = comp;
                };
        }

        void pasteComponent(Entity& tgt)
        {
            if (copiedComponent.type != typeid(void) && copiedComponent.pasteFunction)
            {
                copiedComponent.pasteFunction(tgt);
                copiedComponent.type = typeid(void);
            }
        }

        bool hasCopiedComponent() const
        {
            return copiedComponent.type != typeid(void);
        }


        uint32_t getViewportWidth() { return viewportWidth; }
        uint32_t getViewpotHeight() { return viewportHeight; }

        void setSceneID(UUID id) { sceneID = id; }
        UUID getSceneID() { return sceneID; }
        AudioEngine* getAudio() { return &audioEngine; }
        Camera* getCamera() { return mainCamera; }
        glm::vec2 getViewportDisplay() { return glm::vec2{ 
            viewportWidth, viewportHeight }; }
        
        HMODULE& getDll() { return dll; }
        bool isEditorScene = false;
        std::vector<Entity> toDelete;

    private:

        void deleteEntities();
        UUID sceneID;

        Camera* mainCamera = nullptr;
        Network server;
        Network client;
        entt::registry registry;
        Physics::PhysicsWorld world;
        Scripting::Script scripting;
        AudioEngine audioEngine;
        HMODULE dll;
        friend class Entity;
        //friend class SceneHierarchyPanel;

        std::unordered_map<std::type_index, std::function<void(Entity)>> componentHandlers;

        uint32_t viewportWidth = 0, viewportHeight = 0;

        std::string name;
        
        CopiedComponent copiedComponent;

    };
}

typedef void (*voidFn)();
typedef GameEngine::Transform(*TransformFn)(GameEngine::Entity);
typedef void (*onUpdateFn)(GameEngine::Entity, GameEngine::Timestep);
typedef void (*onCollisionStartFn)(GameEngine::Entity);
typedef void (*onCollisionEndFn)(GameEngine::Entity);
typedef void (*onDestroyFn)(GameEngine::Entity);