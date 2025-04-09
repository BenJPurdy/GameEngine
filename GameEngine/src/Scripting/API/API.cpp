#include "GameEngine_PCH.h"

#include "API.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Logging/Log.h"
#include "Scripting/Scripting.h"
#include "ImGui/imgui.h"
#include "Maths/Maths.h"
#include "GLFW/glfw3.h"
#include "Core/App.h"
#include "Physics/PhysicsBody.h"


namespace GameEngine
{
	struct Transform
	{
		Transform(TransformComponent t)
		{
			position = t.transform; rotation = t.rotation; scale = t.scale;
		}
		TransformComponent toComponent()
		{
			TransformComponent tc;
			tc.transform = position;
			tc.rotation = rotation;
			tc.scale = scale;
			return tc;
		}
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	enum ColliderType
	{
		ColliderType_None,
		ColliderType_Box,
		ColliderType_Circle
	};

	struct ColliderProperties
	{
		//ColliderType type;
		glm::vec2 offset;
		float density;
		float friction;
		float restitution;
		glm::vec2 scale;
	};

	struct RigidbodyProperties
	{
		uint16_t props;
		float mass;
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
		Entity origin = spawner;
		std::string entityName = std::string(name);
		Ref<Scene> scene = Scripting::scripting.currentScene;
		auto& e = scene->createEntity(entityName);
		e.getComponent<TransformComponent>().transform = spawner.getComponent<TransformComponent>().transform + glm::vec3(1.0f, 1.0f, 0.0f);
		auto& rbc = e.addComponent<Rigidbody2dComponent>();
		rbc.properties = spawner.getComponent<Rigidbody2dComponent>().properties;
		auto& ccc = e.addComponent<CircleCollider2dComponent>();
		auto& crc = e.addComponent<CircleRenderComponent>();
		ccc.radius = 0.5f;
		Physics::addRigidBody(spawner.getScene()->getWorld(), e.getComponent<IDComponent>(), rbc);
		Physics::addCircle(spawner.getScene()->getWorld(), e);


		return e;
	}

	SCRIPTAPI Entity scriptNewSpawnEntity(Entity spawner, const char* n)
	{
		std::string name = std::string(n);
		Ref<Scene> scene = Scripting::scripting.currentScene;
		auto& e = scene->createEntity(name);
		return e;
	}

	

	SCRIPTAPI Entity scriptSpawnEntityTransform(Entity spawner, const char* name, Transform t)
	{
		Entity origin = spawner;

		std::string entityName = std::string(name);
		Ref<Scene> scene = Scripting::scripting.currentScene;
		auto& e = scene->createEntity(entityName);
		e.getComponent<TransformComponent>() = t.toComponent();
		auto& rbc = e.addComponent<Rigidbody2dComponent>();
		float scl = glm::length(t.scale);
		rbc.body.mass = 1.0f * scl;
		rbc.properties = spawner.getComponent<Rigidbody2dComponent>().properties;
		auto& ccc = e.addComponent<CircleCollider2dComponent>();
		auto& crc = e.addComponent<CircleRenderComponent>();
		ccc.radius = 0.5f * scl;
		Physics::addRigidBody(spawner.getScene()->getWorld(), e.getComponent<IDComponent>(), rbc);
		Physics::addCircle(spawner.getScene()->getWorld(), e);


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

	//SCRIPTAPI void scriptSetRigidbodyProperties(Entity e, float mass = 1.0f, uint16_t props = 0)
	//{
	//	auto& rb = e.getComponent<Rigidbody2dComponent>();
	//	rb.set(props);
	//	rb.body.mass = mass;
	//	Physics::addRigidBody(e.getScene()->getWorld(), e.getComponent<IDComponent>(), rb);
	//}

	void setBoxColliderProperties(Entity e, ColliderProperties p)
	{
		auto& c = e.getComponent<BoxCollider2dComponent>();
		c.extents = 1.0f * p.scale;
		c.density = p.density;
		c.friction = p.friction;
		c.restitution = p.restitution;
		c.offset = p.offset;
	}

	void setCircleColliderProperties(Entity e, ColliderProperties p)
	{
		auto& c = e.getComponent<CircleCollider2dComponent>();
		c.radius = 1.0f * p.scale.x;
		c.density = p.density;
		c.friction = p.friction;
		c.restitution = p.restitution;
		c.offset = p.offset;
	}

	SCRIPTAPI void scriptSpawnPrefab(Entity spawner, const char* name)
	{
		//TODO (p) set up creation of prefabs, and their spawning
		//return Entity(0, 0);
	}

	SCRIPTAPI void scriptSetColliderProperties(Entity e, ColliderProperties props)
	{
		if (e.hasComponent<BoxCollider2dComponent>())
		{
			setBoxColliderProperties(e, props);
			Physics::addBox(e.getScene()->getWorld(), e);

		}
		else if (e.hasComponent<CircleCollider2dComponent>())
		{
			setCircleColliderProperties(e, props);
			Physics::addCircle(e.getScene()->getWorld(), e);
		}
	}

	SCRIPTAPI void scriptCopyRigidbodyProperties(Entity to, Entity from)
	{
		to.getComponent<Rigidbody2dComponent>() = from.getComponent<Rigidbody2dComponent>();
		Physics::addRigidBody(to.getScene()->getWorld(), to.getComponent<IDComponent>(), 
			to.getComponent<Rigidbody2dComponent>());
	}

	SCRIPTAPI void scriptSetRigidbodyProperties(Entity e, RigidbodyProperties p)
	{
		auto& rb = e.getComponent<Rigidbody2dComponent>();
		rb.set(p.props);
		rb.body.mass = p.mass;
		Physics::addRigidBody(e.getScene()->getWorld(), e.getComponent<IDComponent>(), rb);
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

	SCRIPTAPI const char* scriptGetTag(Entity e)
	{
		if (e.hasComponent<TagComponent>())
		{
			return e.getComponent<TagComponent>().tag.c_str();
		}
	}

	SCRIPTAPI void scriptDestroy(Entity e)
	{
		Scripting::scripting.currentScene->toDelete.push_back(e);
	}

	SCRIPTAPI MousePosition scriptGetMousePos()
	{
		//ImVec2 windowPos = ImGui::GetWindowPos();
		//ImVec2 windowSize = ImGui::GetWindowSize();
		//ImVec2 windowCenter = ImVec2(windowPos.x + windowSize.x * 0.5,
		//	windowPos.y + windowSize.y * 0.5);
		//ImVec2 mousePos = ImGui::GetMousePos();
		//ImVec2 posToCenter = ImVec2(mousePos.x - windowCenter.x,
		//	mousePos.y - windowCenter.y);
		//LOG_TRACE("MousePos: {0}, {1}", posToCenter.x, posToCenter.y);
		auto [x, y] = Input::getMousePosition();
		int winSizex, winSizey;
		auto window = static_cast<GLFWwindow*>(App::get().getWindow().getNativeWindow());
		glfwGetWindowSize(window, &winSizex, &winSizey);
		MousePosition mPos = { (float)x/winSizex, (float)y/winSizey };


		return mPos;
	}

	SCRIPTAPI void playComponentSound(Entity e)
	{
		LOG_TRACE("playing component sound");
		if (e.hasComponent<AudioComponent>())
		{
			LOG_TRACE("sound component exists");
			e.getComponent<AudioComponent>().play();
		}
	}

	
	

}