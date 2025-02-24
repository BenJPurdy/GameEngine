#pragma once
#include "Core/Core.h"
#include "Logging/Log.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace GameEngine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<Scene>&);

		void setContext(const Ref<Scene>&);

		void onImGuiRender();
		Entity getSelectedEntity() const { return selectedContext; }
		void setSelectedEntity(Entity);
	private:
		void drawEntityNode(Entity);
		void rightClickMenu();

		Ref<Scene> context;
		Entity selectedContext;
	};
}