#pragma once

#include "Core/Timestep.h"
#include "entt/entt.hpp"
#include "Core/UUID.h"

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

        void onUpdate(Timestep);

        void destoryEntity(Entity);
        void onViewportResize(uint32_t, uint32_t);

    private:
        entt::registry registry;
        friend class Entity;

        uint32_t viewportWidth = 0, viewportHeight = 0;

    };
}