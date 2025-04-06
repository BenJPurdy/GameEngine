#pragma once
#include "Scene/Scene.h"
#include <filesystem>

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
