#include "GameEngine_PCH.h"

#include "API.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Logging/Log.h"
#include "Scripting/Scripting.h"
#include "ImGui/imgui.h"
#include "Maths/Maths.h"


namespace GameEngine
{
	struct Transform
	{
		Transform(TransformComponent t)
		{
			position = t.transform; rotation = t.rotation; scale = t.scale;
		}
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	struct Render
	{
		glm::vec4 colour;
		union
		{
			glm::vec2 extents;
			float radius;
		};
	};

	struct MousePosition
	{
		float x;
		float y;
	};
	
	SCRIPTAPI void scriptSayHello() { LOG_TRACE("Hello from the script from the engine"); }
	
	SCRIPTAPI void scriptLog(int level, const char* msg)
	{
		switch (level)
		{
		case 0:
			LOG_TRACE("{0}", msg);
			return;
		case 1:
			LOG_INFO("{0}", msg);
			return;
		case 2:
			LOG_WARN("{0}", msg);
			return;
		case 3:
			LOG_ERROR("{0}", msg);
			return;
		case 4:
			LOG_FATAL("{0}", msg);
			return;
		default:
			LOG_WARN("Unregognised log level: {0}", msg);
			return;
		}
	}
	
	SCRIPTAPI Transform scriptGetTransform(Entity e)
	{
 		return Transform(e.getComponent<TransformComponent>());
	}
	SCRIPTAPI void scriptSetTransform(Entity e, Transform t)
	{
		auto& tr = e.getComponent<TransformComponent>();
		tr.transform = t.position;
		tr.rotation = t.rotation;
		tr.scale = t.scale;
		tr.mod = true;
	}
	SCRIPTAPI void scriptAddForce(Entity e, glm::vec3 f)
	{
		auto& rb = e.getComponent<Rigidbody2dComponent>();
		b2Body_ApplyForceToCenter(rb.id, b2Vec2{ f.x, f.y }, true);
	
	}
	SCRIPTAPI void playSound(int index)
	{
		LOG_TRACE("Scene is editor ({0})", 
			(int)Scripting::scripting.currentScene->isEditorScene);
		Scripting::scripting.currentScene->getAudio()->playSound(0);
	}
	SCRIPTAPI bool scriptGetKey(KeyCode k)
	{
		return Input::isKeyPressed(k);
	}
	SCRIPTAPI bool scriptGetMouse(MouseCode m)
	{
		return Input::isMouseButtonPressed(m);
	}
	SCRIPTAPI Entity scriptGetEntity(const char* n)
	{
		std::string name = std::string(n);
		Scene* scenePtr = Scripting::scripting.currentScene.get();
		auto& view = scenePtr->getRegistry().view<TagComponent>();
		for (auto& e : view)
		{
			auto& tag = view.get<TagComponent>(e);
			if (tag.tag == name)
			{
				Entity et = Entity(e, Scripting::scripting.currentScene.get());
				LOG_TRACE("Entity {1} got at {0}", (uint32_t)et.getEntt(), et.getComponent<TagComponent>().tag);
				
				return et;
			}
		}
	}
	SCRIPTAPI Entity scriptSpawnEntity(Entity spawner, const char* name)
	{
		std::string entityName = std::string(name);
		Ref<Scene> scene = Scripting::scripting.currentScene;
		auto& e = scene->createEntity(entityName);
		e.getComponent<TransformComponent>().transform = spawner.getComponent<TransformComponent>().transform + glm::vec3(1.0f, 1.0f, 0.0f);
		auto& rbc = e.addComponent<Rigidbody2dComponent>();
		rbc.properties = spawner.getComponent<Rigidbody2dComponent>().properties;
		auto& ccc = e.addComponent<CircleCollider2dComponent>();
		auto& crc = e.addComponent<CircleRenderComponent>();
		ccc.radius = 0.5f;


		return e;
	}

	SCRIPTAPI Render scriptGetRender(Entity e)
	{
		if (e.hasComponent<CircleRenderComponent>())
		{
			auto& c = e.getComponent<CircleRenderComponent>();
			Render r;
			r.colour = c.colour;
			r.radius = c.radius;
			return r;
		}
		else if (e.hasComponent<SpriteRenderComponent>())
		{
			auto& c = e.getComponent<SpriteRenderComponent>();
			Render r;
			r.colour = c.colour;
			return r;
		}
	}

	SCRIPTAPI void scriptAddComponent(Entity e, ComponentType c)
	{
		Entity en = Entity(e);
		switch (c)
		{
		case ComponentType::Component_ID:
			en.addComponentFromScript<IDComponent>(); return;
		case ComponentType::Component_Tag:
			en.addComponentFromScript<TagComponent>(); return;
		case ComponentType::Component_Transform:
			en.addComponentFromScript<TransformComponent>(); return;
		case ComponentType::Component_Camera:
			en.addComponentFromScript<CameraComponent>(); return;
		case ComponentType::Component_SpriteRender:
			en.addComponentFromScript<SpriteRenderComponent>(); return;
		case ComponentType::Component_Script:
			en.addComponentFromScript<ScriptComponent>(); return;
		case ComponentType::Component_CircleRender:
			en.addComponentFromScript<CircleRenderComponent>(); return;
		case ComponentType::Component_Rigidbody2d:
			en.addComponentFromScript<Rigidbody2dComponent>(); return;
		case ComponentType::Component_BoxCollider2d:
			en.addComponentFromScript<BoxCollider2dComponent>(); return;
		case ComponentType::Component_CircleCollider2d:
			en.addComponentFromScript<CircleCollider2dComponent>(); return;
			
		}
	}

	SCRIPTAPI void scriptAssignScriptName(Entity e, const char* n)
	{
		Scene* sc = Scripting::scripting.currentScene.get();
		std::string name = std::string(n);
		auto& sC = e.getComponent<ScriptComponent>();
		sC.script = name;
		Scripting::populatePointers(sc->getDll(), sC);
		OnStartFn f = (OnStartFn)sC.onStartPtr;
		if (f)
			f(e);
		else
		{
			LOG_WARN("Entity {0} does not impliment an onStart function\nMake sure this is intended", 
				e.getComponent<TagComponent>().tag);
		}
		
	}

	SCRIPTAPI void scriptDestroy(Entity e)
	{
		Scripting::scripting.currentScene->toDelete.push_back(e);
	}

	SCRIPTAPI MousePosition scriptGetMousePos()
	{
		auto [x, y] = Input::getMousePosition();
		MousePosition mPos = { x, y };


		return mPos;
	}
	

}