#pragma once
#include <box2d/box2d.h>
#include <Scene/Components.h>
#include <unordered_map>
#include "Scene/Components.h"
#include "Core/UUID.h"
#include <entt/entt.hpp>

namespace GameEngine
{
	class Entity;
	namespace Physics
	{
		class PhysicsWorld
		{
		public:
			b2WorldId id{ 0 };
			//map of dynamic objects to UUID's
			std::unordered_map<uint64_t, b2BodyId> rigidBodies;

			//In the beginning the [b2World] was created.
			//This has made a lot of people very angry and been widely regarded as a bad move.
			bool create()
			{
				//notes, world uses +y for up
				//we're using defaults for now
				b2WorldDef wd = b2DefaultWorldDef();
				id = b2CreateWorld(&wd);
				return true;
			}

			//As you will no doubt be aware, the plans for development of the outlying regions of the [memory] 
			//	require the building of a [new gpu partition] through your [Game Engine]
			//And regrettably, your [b2World] is one of those scheduled for demolition.
			void destory()
			{
				//we don't need to invalidate the id's as they get regenerated blindly
				b2DestroyWorld(id);
			}
		};

		void simulateWorld(PhysicsWorld w);

		//all [shapes] are made up
		b2ShapeDef setShapeDefs(Collider2d&);
		

		void syncToWorld(PhysicsWorld, entt::registry);

		void syncToRender(PhysicsWorld, entt::registry);

		//In the begining there was nothing, which exploded
		void addCircle(PhysicsWorld, Entity);

		
		//!!
		void addBox(PhysicsWorld, Entity);

		//A rigidbody [The box2d documentation] says, is about the most massively useful thing a [physics engine] can have.
		void addRigedBody(PhysicsWorld, IDComponent, Rigidbody2dComponent&);

	}
}
