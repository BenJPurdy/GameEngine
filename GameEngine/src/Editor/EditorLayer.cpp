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
		shaders = createRef<ShaderLibrary>();
		shaders->load("FlatColour", "assets/shaders/FlatColour");
	}

	void EditorLayer::onDetatch()
	{

	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		//framebuffer->bind();
		renderer3d->preProcessing();
		camera.onUpdate(ts);

		auto shader = shaders->get("FlatColour");
		shader->bind();
		shader->setFloat4("colour", { 0.2f, 0.5f, 0.3f, 1.0f });
		shader->setMat4("viewProjection", camera.getViewProjection());
		shader->setMat4("transform", glm::mat4(1.0f));
		renderer3d->renderTriangle();

		//framebuffer->unbind();
	}

	void EditorLayer::onEvent(Event& e)
	{
		camera.onEvent(e);
	}
}