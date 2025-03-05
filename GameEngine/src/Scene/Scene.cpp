#include "GameEngine_PCH.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

#include "Renderer/2D/Renderer2D.h"

namespace GameEngine
{
    Scene::Scene(const std::string& _name, bool _isEditorScene)
        : name(_name), isEditorScene(_isEditorScene)
    {
        registerComponentHandler<CameraComponent>([](Entity e, CameraComponent& c)
            {
                LOG_INFO("Camera component added");
                if (auto* scenePtr = e.getScene())
                {
                    c.camera.setViewportSize(scenePtr->getViewportWidth(), scenePtr->getViewpotHeight());
                }
            });
    }

    Scene::~Scene() {}

    Ref<Scene> Scene::copy(Ref<Scene> other)
    {
        Ref<Scene> newScene = createRef<Scene>();
        copyTo(other, newScene);
        return newScene;
    }

    void Scene::copyTo(Ref<Scene> src, Ref<Scene> dst)
    {
        if (dst == nullptr) dst = createRef<Scene>();

        dst->viewportHeight = src->viewportHeight;
        dst->viewportWidth = src->viewportWidth;

        std::unordered_map<UUID, entt::entity> enttMap;

        auto& srcSceneRegistry = src->registry;
        auto& dstSceneRegistry = dst->registry;

        auto idView = srcSceneRegistry.view<IDComponent>();

        for (auto e : idView)
        {
            UUID uuid = srcSceneRegistry.get<IDComponent>(e).id;
            const auto& tag = srcSceneRegistry.get<TagComponent>(e).tag;
            Entity e = dst->createEntityWithUUID(uuid, tag);

            enttMap[uuid] = e.entityHandle;
        }

        copyComponent<TagComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<SpriteRenderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        //add more here as and when more componenets are added
        //or make a list of types idk

        dst->isEditorScene = false;
    }

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

    Entity Scene::duplicateEntity(Entity e)
    {
        Entity newEntity;
        if (e.hasComponent<TagComponent>())
        {
            newEntity = createEntity(e.getComponent<TagComponent>().tag);
        }
        else
        {
            newEntity = createEntity();
        }

        copyComponentIfExists<TransformComponent>(newEntity.entityHandle, registry, e);
        copyComponentIfExists<CameraComponent>(newEntity.entityHandle, registry, e);
        copyComponentIfExists<SpriteRenderComponent>(newEntity.entityHandle, registry, e);

        return newEntity;
    }

    void Scene::destoryEntity(Entity e)
    {
        registry.destroy(e);
    }

    //void Scene::onUpdate(Timestep t)
    //{
    //    Camera* mainCamera = nullptr;
    //    glm::mat4 cameraTransform;
    //    {
    //        auto view = registry.view<TransformComponent, CameraComponent>();
    //
    //        for (auto e : view)
    //        {
    //            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(e);
    //
    //            if (camera.primary)
    //            {
    //                mainCamera = &camera.camera;
    //                cameraTransform = transform.getTransform();
    //                break;
    //            }
    //        }
    //    }
    //
    //    if (mainCamera)
    //    {
    //        Render2d::beginScene(mainCamera->GetProjection(), cameraTransform);
    //        
    //        {
    //            auto group = registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
    //            for (auto e : group)
    //            {
    //                auto [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(e);
    //                Render2d::drawSprite(transform.getTransform(), sprite, (int)e);
    //            }
    //        }
    //        Render2d::endScene();
    //    }
    //}

    void Scene::onUpdateEditor(Timestep ts, EditorCamera& camera)
    {
        Render2d::beginScene(camera);

        {
            auto group = registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(entity);
                Render2d::drawSprite(transform.getTransform(), sprite, (int)entity);
            }

            Render2d::endScene();
        }
    }

    void Scene::onUpdateRuntime(Timestep ts)
    {
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

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
                    Render2d::drawSprite(transform.getTransform(), sprite, (int)e);
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

    void Scene::onRuntimeStart()
    {
        //contains pysworld setup and all that good stuff
    }

    void Scene::onRuntimeStop()
    {
        //cleanup all that good stuff
    }
}