#include "GameEngine_PCH.h"

#include "EditorLayer.h"

namespace GameEngine
{
	EditorLayer::EditorLayer() : Layer("EditorLayer")
	{

	}

	void EditorLayer::onAttach()
	{
		camera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

		FramebufferSpecification spec;
		spec.w = 1280;
		spec.h = 720;
		framebuffer = createRef<Framebuffer>(spec);

		renderer3d = createRef<Render3d>();
		renderer3d->setupTriangle();
	}

	void EditorLayer::onDetatch()
	{

	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		//framebuffer->bind();
		renderer3d->preProcessing();
		camera.onUpdate(ts);
		renderer3d->renderTriangle();

		//framebuffer->unbind();
	}

	void EditorLayer::onEvent(Event& e)
	{
		camera.onEvent(e);
	}
}