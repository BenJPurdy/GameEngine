#pragma once
#include <box2d/box2d.h>
#include <Scene/Components.h>
#include <unordered_map>
#include "Core/UUID.h"
#include <entt/entt.hpp>

namespace GameEngine
{
	class Entity;
	class Scene;
	namespace Physics
	{
		//This holds the basic box2d variables and a pointer to the scene,
		// as well as functions relating to the b2World.
		//
		class PhysicsWorld
		{
		public:
			b2WorldId id{ 0 };
			//map of dynamic objects to UUID's
			std::unordered_map<uint64_t, b2BodyId> rigidBodies;
			Scene* scene;

			//In the beginning the [b2World] was created.
			//This has made a lot of people very angry and been widely regarded as a bad move.
			//(Makes the physics world)
			bool create(Scene*);

			//As you will no doubt be aware, the plans for development of the outlying regions of the [memory] 
			//	require the building of a [new shared memory block] through your [Game Engine]
			//And regrettably, your [b2World] is one of those scheduled for demolition.
			void destory()
			{
				//we don't need to invalidate the id's as they get regenerated blindly
				b2DestroyWorld(id);
			}
		};


		void simulateWorld(PhysicsWorld& w, entt::registry& r);

		//all [shapes] are made up
		b2ShapeDef setShapeDefs(Collider2d&);
		

		void syncToWorld(PhysicsWorld&, entt::registry&);

		void syncToRender(PhysicsWorld&, entt::registry&);

		// TOOD (P) set this up properly, current solution is messy
		void registerEntity(PhysicsWorld&, Entity);

	
		void addCircle(PhysicsWorld&, Entity);

		
		//!!
		void addBox(PhysicsWorld&, Entity);

		//A rigidbody [The box2d documentation] says, 
		// is about the most massively useful thing a [physics engine] can have.
		void addRigidBody(PhysicsWorld&, IDComponent&, Rigidbody2dComponent&);

	}
}



//Comments adapted from The Hitchhiker's Guide to the Galaxy (Douglas Adams), Diskworld (Sir Terry Pratchett), 
