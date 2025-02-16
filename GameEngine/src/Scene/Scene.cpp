#include "GameEngine_PCH.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

namespace GameEngine
{
    Scene::Scene()
    {

    }

    Scene::~Scene() {}

    Entity Scene::createEntity(const std::string& name)
    {
        return createEntityWithUUID(UUID(), name);
    }

    Entity Scene::createEntityWithUUID(UUID uuid, const std::string& name)
    {
        auto e = Entity{ registry.create(), this };
        e.addComponent<TransformComponent>();
        e.addComponent<IDComponent>(uuid);
        auto& tag = e.getComponent<TagComponent>();
        tag.tag = name.empty() ? "Entity" : name;

        return e;
    }

    void Scene::onUpdate(Timestep t)
    {

    }
}