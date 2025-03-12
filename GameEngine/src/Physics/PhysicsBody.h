#pragma once
#include <box2d/box2d.h>
#include <Scene/Components.h>
#include <unordered_map>
#include "Scene/Components.h"
#include "Core/UUID.h"

namespace GameEngine
{
	namespace Physics
	{
		class PhysicsWorld
		{
		public:
			b2WorldId id{ 0 };
			std::unordered_map<uint64_t, b2BodyId> rigidBodies;
		}physicsWorld;


		bool createWorld()
		{
			//notes, world uses +y for up
			//we're using defaults for now
			b2WorldDef wd = b2DefaultWorldDef();
			physicsWorld.id = b2CreateWorld(&wd);
			return true;
		}

		void addBox(IDComponent id, BoxCollider2dComponent& c)
		{
			
		}

		void addRigedBody(IDComponent id, Rigidbody2dComponent& c)
		{

		}


	}
}
