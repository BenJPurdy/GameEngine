#pragma once
#include "GameEngine.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/Panels/stats.h"
#include <filesystem>


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
		
		bool onMouseButtonPressed(MouseButtonPressedEvent);
		bool onKeyPressed(KeyPressedEvent e);

		void newScene();
		void openScene();
		void openScene(const std::filesystem::path&);
		void saveScene();
		void saveSceneAs();
		void serialiseScene(Ref<Scene>, const std::filesystem::path&);

		void onDuplicateEntity();
		void onDeleteEntity();

		void onScenePlay();
		void onSceneStop();

		void onServerStart(bool = false);
		void onServerStop(bool = false);

		EditorCamera camera;
		Ref<Framebuffer> framebuffer;
		Ref<ShaderLibrary> shaders;
		//Ref<Render3d> renderer3d;
		//Ref<Render2d> render2d;
		Ref<Scene> activeScene;
		Ref<Scene> editorScene, runtimeScene;
		std::filesystem::path editorSceneFilePath;

		int gizmoType = -1;

		bool viewportFocus = false;
		bool viewportHover = false;
		bool runServer = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];

		Entity hoveredEntity;

		
		SceneHierarchyPanel sceneHierarchy;
		Stats statsPannel;

		enum class SceneState
		{
			Edit = 0,
			Play = 1,
			Pause = 2,
			Simulate = 3,
			Server = 4,


			SceneStateSize
		};

		SceneState sceneState = SceneState::Edit;
	};
}