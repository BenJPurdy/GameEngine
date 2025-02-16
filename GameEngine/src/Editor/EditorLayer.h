#pragma once
#include "GameEngine.h"


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
		Ref<Render3d> renderer3d;
		Ref<ShaderLibrary> shaders;
		Ref<Render2d> render2d;

		bool viewportFocus = false;
		bool viewportHover = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };

		glm::vec4 testColour = {0.3f, 0.4f, 0.9f, 1.0f};
		glm::vec2 testSize = {0.5f, 0.5f};
	};
}