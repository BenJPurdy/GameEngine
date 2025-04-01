#include "GameEngine_PCH.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

#include "Renderer/2D/Renderer2D.h"

#include "Physics/PhysicsBody.h"

#include "Scripting/Scripting.h"

#include "Core/Input.h"

namespace GameEngine
{
    Scene::Scene(const std::string& _name, bool _isEditorScene)
        : name(_name), isEditorScene(_isEditorScene)
    {
        audioEngine.init();
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
        copyComponent<CircleRenderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<Rigidbody2dComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<BoxCollider2dComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<CircleCollider2dComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<ScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

        //for (auto& i : componentTypes)
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
        copyComponentIfExists<CircleRenderComponent>(newEntity.entityHandle, registry, e);
        copyComponentIfExists<Rigidbody2dComponent>(newEntity.entityHandle, registry, e);
        copyComponentIfExists<BoxCollider2dComponent>(newEntity.entityHandle, registry, e);
        copyComponentIfExists<CircleCollider2dComponent>(newEntity.entityHandle, registry, e);
        copyComponentIfExists<ScriptComponent>(newEntity.entityHandle, registry, e);
        return newEntity;
    }

    void Scene::destoryEntity(Entity e)
    {
        registry.destroy(e);
    }

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
        }
        if (Input::isMouseButtonPressed(Mouse::ButtonMiddle))
        {
            audioEngine.playTestSound();
        }

        audioEngine.update();
        {
            auto view = registry.view<TransformComponent, CircleRenderComponent>();
            for (auto e : view)
            {
                auto [transform, circle] = view.get<TransformComponent, CircleRenderComponent>(e);
                Render2d::drawCircle(transform.getTransform(), circle.colour, circle.thickness, circle.fade, (int)e);
            }
        }
        Render2d::endScene();
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

        {
            auto view = registry.view<ScriptComponent>();
            for (auto& e : view)
            {
                
                auto& SC = view.get<ScriptComponent>(e);
                onUpdateFn uF = (onUpdateFn)SC.onUpdatePtr;
                if (uF)
                {
                    Entity local = Entity(e, this);
                    uF(local, ts.getSeconds());
                }
                else
                {
                    LOG_WARN("Failed to get update function");
                }
            }
        }
        //if ()
        Physics::simulateWorld(world, registry);
        

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
            
            {
                auto view = registry.view<TransformComponent, CircleRenderComponent>();
                for (auto e : view)
                {
                    auto [transform, circle] = view.get<TransformComponent, CircleRenderComponent>(e);
                    Render2d::drawCircle(transform.getTransform(), circle.colour, circle.thickness, circle.fade, (int)e);
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
        LOG_TRACE("Current scene ptr: {0}", (intptr_t)this);
        //scripting.currentScene = this;
        if (!scripting.compileScripts())
        {
            LOG_FATAL("Failed to compile scripts into dll, using previous version");
        }
        else
        {
            std::cout << "\x1b[44m \x1b[37mFINISHED COMPILING SCRIPTS\x1b[0m\n";
        }
        Scripting::loadLib(dll);
        LOG_TRACE("Audio engine has {0} sounds", audioEngine.sounds.size());
        LOG_TRACE("DLLHandle is {0}", (intptr_t)dll);
        world.create(this);
        {
            auto& view = registry.view<IDComponent, Rigidbody2dComponent>();
            for (auto& e : view)
            {
                auto& [id, rb] = view.get<IDComponent, Rigidbody2dComponent>(e);
                Physics::addRigidBody(world, id, rb);
            }
        }
        {
            auto& view = registry.view<Rigidbody2dComponent, BoxCollider2dComponent>();
            for (auto& e : view)
            {
                auto [id, rb] = view.get<Rigidbody2dComponent, BoxCollider2dComponent>(e);
                Physics::addBox(world, Entity(e, this));
            }
        }
        {
            auto& view = registry.view<Rigidbody2dComponent, CircleCollider2dComponent>();
            for (auto& e : view)
            {
                auto [id, rb] = view.get<Rigidbody2dComponent, CircleCollider2dComponent>(e);
                Physics::addCircle(world, Entity(e, this));
            }
        }

        //make a bunch of function pointers by looping over entities
        {
            auto& view = registry.view<ScriptComponent>();
            for (auto& e : view)
            {
                auto& sc = view.get<ScriptComponent>(e);
                Scripting::populatePointers(dll, sc);
            }
        }
        
    }

    void Scene::onRuntimeStop()
    {
        world.destory();
        audioEngine.destory();
        //setting mod to true to make sure that all entities are loaded into the physics next runtime
        //:)
        auto& view = registry.view<TransformComponent>();
        for (auto& e : view)
        {
            view.get<TransformComponent>(e).mod = true;
        }
        Scripting::freeDll(dll);
        //scripting.unloadLib();
        scripting.currentScene = nullptr;
    }
}