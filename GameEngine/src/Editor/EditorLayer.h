#pragma once
#include "GameEngine.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Editor/Panels/SceneHierarchyPanel.h"


namespace GameEngine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetatch() override;
		void onUpdate(Timestep) override;
		virtual void onImGuiRender() override;
		void onEvent(Event&) override;

	private:
		
		EditorCamera camera;
		Ref<Framebuffer> framebuffer;
		Ref<ShaderLibrary> shaders;
		//Ref<Render3d> renderer3d;
		//Ref<Render2d> render2d;
		Ref<Scene> activeScene;

		int guizmoType = -1;

		bool viewportFocus = false;
		bool viewportHover = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };

		
		SceneHierarchyPanel sceneHierarchy;
	};
}