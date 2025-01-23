#pragma once

#include "Core/Core.h"
#include "Core/Timestep.h"
#include <string>

namespace GameEngine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {};
		virtual void onDetatch() {};
		virtual void onUpdate(Timestep t) {};
	private:
		std::string name;
	};
}
