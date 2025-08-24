#pragma once

#include "Core/Core.h"
#include "Scene/Entity.h"

namespace GameEngine
{
	class InspectorPanel
	{
	public:
		InspectorPanel() {};
		~InspectorPanel() = default;

		void drawComponents(Entity);
		void drawAddComponents(Entity);

	private:
		template<typename T>
		void drawAddComponentMenuItem(Entity, const char*);

		std::string rigidbodyType(Entity);
		void checkAndCopyBuffer(std::string&, char*);
		void menuItemTextInput(const char*, std::string&, char*);
	};
}
