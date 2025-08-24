#include "GameEngine_PCH.h"
#include "Entity.h"

namespace GameEngine
{
	Entity::Entity(entt::entity _h, Scene* s) :
		entityHandle(_h), scene(s)
	{}
}