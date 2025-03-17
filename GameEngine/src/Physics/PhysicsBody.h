#pragma once
#include <box2d/box2d.h>
#include <Scene/Components.h>
#include <unordered_map>
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Core/UUID.h"

namespace GameEngine
{
	namespace Physics
	{
		class PhysicsWorld
		{
		public:
			b2WorldId id{ 0 };
			//map of dynamic objects to UUID's
			std::unordered_map<uint64_t, b2BodyId> rigidBodies;

			bool create()
			{
				//notes, world uses +y for up
				//we're using defaults for now
				b2WorldDef wd = b2DefaultWorldDef();
				id = b2CreateWorld(&wd);
				return true;
			}
		};


		

		void addBox(PhysicsWorld w, Entity e, BoxCollider2dComponent& c)
		{
			auto& rb = e.getComponent<Rigidbody2dComponent>();
			
			
			b2BodyDef bodyDef;

			bodyDef.type = (b2BodyType)rb.properties;

			b2BodyId body = b2CreateBody(w.id, &bodyDef);
			
		}

		void addRigedBody(IDComponent id, Rigidbody2dComponent& c)
		{

		}


	}
}
