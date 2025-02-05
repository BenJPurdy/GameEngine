#pragma once
#include "GameEngine.h"
#include "Renderer/Camera/EditorCamera.h"


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
		void onEvent(Event&) override;

	private:
		bool onWindowResize(WindowResizeEvent&);


		EditorCamera camera;
		Ref<Framebuffer> framebuffer;
		Ref<Render3d> renderer3d;
		Ref<ShaderLibrary> shaders;
	};
}