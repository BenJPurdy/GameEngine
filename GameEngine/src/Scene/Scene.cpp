#include "GameEngine_PCH.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

#include "Renderer/2D/Renderer2D.h"

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

    Entity Scene::createEntityWithUUID(UUID uid, const std::string& name)
    {
        auto e = Entity{ registry.create(), this };
        e.addComponent<TransformComponent>();
        e.addComponent<IDComponent>(uid); //FIGURE OUT ISSUE HERE
        auto& tag = e.addComponent<TagComponent>();
        tag.tag = name.empty() ? "Entity" : name;

        LOG_INFO("{0}, ID: {1}", tag.tag, (uint64_t)e.getComponent<IDComponent>().id);

        return e;
    }

    void Scene::destoryEntity(Entity e)
    {
        registry.destroy(e);
    }

    void Scene::onUpdate(Timestep t)
    {
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = registry.view<TransformComponent, CameraComponent>();

            for (auto e : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(e);

                if (camera.primary)
                {
                    mainCamera = &camera.camera;
                    cameraTransform = transform.getTransform();
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Render2d::beginScene(mainCamera->GetProjection(), cameraTransform);
            
            {
                auto group = registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
                for (auto e : group)
                {
                    auto [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(e);
                    Render2d::drawSprite(transform.getTransform(), sprite);
                }
            }
            Render2d::endScene();
        }
    }

    void Scene::onViewportResize(uint32_t w, uint32_t h)
    {
        viewportWidth = w;
        viewportHeight = h;

        auto view = registry.view<CameraComponent>();
        for (auto e : view)
        {
            auto& cc = view.get<CameraComponent>(e);
            if (cc.fixedAspect == false)
            {
                cc.camera.setViewportSize(w, h);
            }
        }
    }
}