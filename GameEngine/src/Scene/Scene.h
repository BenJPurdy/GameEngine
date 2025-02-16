#pragma once

#include "Core/Timestep.h"
#include "entt/entt.hpp"

namespace GameEngine
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity createEntity(const std::string& name = std::string());
        Entity createEntityWithUUID(UUID, const std::string&);

        void onUpdate(Timestep);

    private:
        entt::registry registry;
        friend class Entity;

    };
}