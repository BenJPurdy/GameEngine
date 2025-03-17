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
			if (!e.hasComponent<Rigidbody2dComponent>()) return;
			auto& rb = e.getComponent<Rigidbody2dComponent>();
			b2Polygon poly = b2MakeBox(c.extents.x, c.extents.y);
			b2ShapeDef shape = b2DefaultShapeDef();

			b2CreatePolygonShape(rb.id, &shape, &poly);
			
			
		}

		void addRigedBody(PhysicsWorld w, IDComponent id, Rigidbody2dComponent& c)
		{
			b2BodyId null = b2_nullBodyId;
			
			b2BodyDef def = b2DefaultBodyDef();
			def.type = (b2BodyType)(c.properties & 0b00000111);
			def.userData = (void*)(uint64_t)id.id;
			c.id = b2CreateBody(w.id, &def);
		}


	}
}
