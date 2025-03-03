#pragma once

#include "Core/Timestep.h"
#include "entt/entt.hpp"
#include "Core/UUID.h"

#include "Components.h"
#include <typeindex>

namespace GameEngine
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity createEntity(const std::string& name = std::string());
        Entity createEntityWithUUID(UUID, const std::string& = "");

        entt::registry& getRegistry() { return registry; }

        void onUpdate(Timestep);

        void destoryEntity(Entity);
        void onViewportResize(uint32_t, uint32_t);

        template<typename T>
        void registerComponentHandler(std::function<void(Entity, T&)> handler)
        {
            componentHandlers[typeid(T)] = [handler](Entity e)
                {
                    handler(e, e.getComponent<T>());
                };
        }

        template<typename T>
        void onComponentAdded(Entity e, T& c)
        {
            
        }


    private:
        entt::registry registry;
        friend class Entity;

        std::unordered_map<std::type_index, std::function<void(Entity)>> componentHandlers;

        uint32_t viewportWidth = 0, viewportHeight = 0;

    };
}