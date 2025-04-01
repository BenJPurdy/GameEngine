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
				return Entity(e, scenePtr);
			}
		}
	}

}