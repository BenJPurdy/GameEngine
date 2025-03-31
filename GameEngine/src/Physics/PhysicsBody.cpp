#include "GameEngine_PCH.h"
#include "PhysicsBody.h"
#include "Scene/Entity.h"
#include "Core/Timestep.h"
#include "Scene/Scene.h"

namespace GameEngine
{
	bool Physics::PhysicsWorld::create(Scene* s)
	{
		//notes, world uses +y for up
		//we're using defaults for now
		b2WorldDef wd = b2DefaultWorldDef();
		scene = s;
		id = b2CreateWorld(&wd);

		return true;
	}

	void Physics::syncToWorld(PhysicsWorld& w, entt::registry& r)
	{
		auto view = r.view<TransformComponent, Rigidbody2dComponent>();
		for (auto& e : view)
		{
			auto [transform, rb] = view.get<TransformComponent, Rigidbody2dComponent>(e);
			//if (rb.has(PhysicsProperties::PhysProps_Static))
			//{
			//	continue;
			//}
			if (!transform.mod) continue;
			else
			{
				b2Vec2 p{ transform.transform.x, transform.transform.y };
				b2Rot r = b2MakeRot(transform.rotation.z);
				b2Body_SetTransform(rb.id, p, r);
				transform.mod = false;
			}

			
		}
	}

	void Physics::syncToRender(PhysicsWorld& w, entt::registry& r)
	{
		auto view = r.view<TransformComponent, Rigidbody2dComponent>();
		for (auto& e : view)
		{
			auto [transform, rb] = view.get<TransformComponent, Rigidbody2dComponent>(e);
			if (rb.has(PhysicsProperties::PhysProps_Static))
			{
				continue;
			}
			//if (!transform.mod) continue;

			b2Transform t = b2Body_GetTransform(rb.id);

			transform.transform = glm::vec3{ t.p.x, t.p.y, 0.0f };
			transform.rotation.z = b2Rot_GetAngle(t.q);
		}
	}

	void Physics::addCircle(PhysicsWorld& w, Entity e)
	{
		if (!e.hasComponent<Rigidbody2dComponent>()) return;
		auto& c = e.getComponent<CircleCollider2dComponent>();
		auto& rb = e.getComponent<Rigidbody2dComponent>();
		b2Circle circle;
		circle.center = b2Vec2{ c.offset.x, c.offset.y };
		circle.radius = c.radius; 
		b2ShapeDef shape = setShapeDefs(c);

		b2CreateCircleShape(rb.id, &shape, &circle);

	}

	//!!
	void Physics::addBox(PhysicsWorld& w, Entity e)
	{
		if (!e.hasComponent<Rigidbody2dComponent>()) return;

		auto& rb = e.getComponent<Rigidbody2dComponent>();
		auto& c = e.getComponent<BoxCollider2dComponent>();
		auto& t = e.getComponent<TransformComponent>();
		b2Polygon poly = b2MakeBox((c.extents.x / 2) * std::abs(t.scale.x), (c.extents.y / 2) * std::abs(t.scale.y));
		b2ShapeDef shape = setShapeDefs(c);
		b2CreatePolygonShape(rb.id, &shape, &poly);

	}

	
	void Physics::addRigidBody(PhysicsWorld& w, IDComponent id, Rigidbody2dComponent& c)
	{
		b2BodyId null = b2_nullBodyId;
		b2BodyDef def = b2DefaultBodyDef();
		
		def.type = (b2BodyType)c.getBodyType();
		def.userData = (void*)(uint64_t)id.id;
		c.id = b2CreateBody(w.id, &def);
	}
	b2ShapeDef Physics::setShapeDefs(Collider2d& c)
	{
		b2ShapeDef s = b2DefaultShapeDef();
		s.density = c.density;
		s.friction = c.friction;
		s.restitution = c.restitution;
		s.isSensor = c.isSensor;
		return s;
	}

	void Physics::simulateWorld(PhysicsWorld& w, entt::registry& r)
	{
		//pos = 3x float
		//rot = 4x float
		Physics::syncToWorld(w, r);
		b2World_Step(w.id, 0.01667f, 4);

		

		b2SensorEvents sensorEvents = b2World_GetSensorEvents(w.id);
		b2ContactEvents contactEvents = b2World_GetContactEvents(w.id);
		for (int i = 0; i < sensorEvents.beginCount; i++)
		{
			b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
			void* data = b2Shape_GetUserData(beginTouch->visitorShapeId);
			entt::entity id = (entt::entity)(uint32_t)data;
			Entity e = Entity(id, w.scene);
			if (e.hasComponent<ScriptComponent>())
			{
				ScriptComponent eSC = e.getComponent<ScriptComponent>();
				//eSC.onCollisionEnterPtr();
			}
			
		}
		

		Physics::syncToRender(w, r);
	}
}
