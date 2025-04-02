#include "GameEngine_PCH.h"

#include "API.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Logging/Log.h"
#include "Scripting/Scripting.h"


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
	
	SCRIPTAPI void speak() {}
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

}