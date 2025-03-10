#pragma once
#include "Scene/Scene.h"

namespace GameEngine
{
	class SceneSerialiser
	{
	public:
		SceneSerialiser(const Ref<Scene>&);

		void serialise(const std::string&);
		bool deserialise(const std::string&);
	private:
		Ref<Scene> scene;
	};
}
