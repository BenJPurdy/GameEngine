#include "GameEngine_PCH.h"

#include "EditorLayer.h"
#include <glm/gtx/string_cast.hpp>

namespace GameEngine
{
	EditorLayer::EditorLayer() : Layer("EditorLayer")
	{

	}

	void EditorLayer::onAttach()
	{
		camera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		//camera = EditorCamera();
		

		FramebufferSpecification spec;
		spec.w = 1280;
		spec.h = 720;
		framebuffer = Framebuffer::create(spec);

		renderer3d = createRef<Render3d>();
		renderer3d->setupTriangle();

		render2d = createRef<Render2d>();
		render2d->init();

		shaders = createRef<ShaderLibrary>();
		shaders->load("flatColour", "assets/shaders/flatColour");
	}

	void EditorLayer::onDetatch()
	{

	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		//framebuffer->bind();
		
		//renderer3d->preProcessing();

		camera.onUpdate(ts);

		//LOG_TRACE(glm::to_string(camera.GetPosition()) + " " + glm::to_string(camera.GetRightDirection()));


		//
		render2d->beginScene(camera);

		auto shader = shaders->get("flatColour");
		//LOG_TRACE(shader->getName());
		if (0)
		{
			shader->bind();
			shader->setFloat4("colour", { 0.2f, 0.8f, 0.3f, 1.0f });
			shader->setMat4("viewProjection", camera.getViewProjection());
			////LOG_WARN(glm::to_string(camera.getViewProjection()));
			shader->setMat4("transform", glm::mat4(1.0f));
		}
		//renderer3d->renderTriangle();

		for (float c = -5; c < 5; c++)
		{
			for (float r = -5; r < 5; r++)
			{
				render2d->drawQuad(glm::vec3(c, r, 0.0f), glm::vec2(0.5f, 0.5f), { 0.2f, 0.8f, 0.2f, 1.0f });
			}
		}
		render2d->endScene();
		auto stats = render2d->getStats();
		auto error = glGetError();
		if (error) { LOG_ERROR("GL Error: {0}", error); }
		LOG_INFO("Draw Calls: {0}", stats.drawCalls);
		//framebuffer->unbind();
	}

	void EditorLayer::onEvent(Event& e)
	{
		camera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowResizeEvent>(
			std::bind(&EditorLayer::onWindowResize, this, std::placeholders::_1)
		);
	}

	bool EditorLayer::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			return false;
		}
		
		LOG_INFO("Window size: {0}, {1}", e.getWidth(), e.getHeight());
		renderer3d->updateViewportSize(e.getWidth(), e.getHeight());

		return false;
	}
}