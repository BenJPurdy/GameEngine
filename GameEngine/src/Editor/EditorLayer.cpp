#include "GameEngine_PCH.h"

#include "EditorLayer.h"
#include "ImGui/ImGuiLib.h"

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

		activeScene = createRef<Scene>();
		sceneHierarchy.setContext(activeScene);

		//renderer3d = createRef<Render3d>();
		//renderer3d->setupTriangle();
		//
		//render2d = createRef<Render2d>();
		//render2d->init();
		//render2d->setClearColour({ 0.2f, 0.2f, 0.2f, 1.0f });

		shaders = createRef<ShaderLibrary>();
		shaders->load("flatColour", "assets/shaders/flatColour");



		

		testCamera = activeScene->createEntity("TestCamera");
		testEntity = activeScene->createEntity("TestQuad");

		testCamera.addComponent<CameraComponent>();
		testEntity.addComponent<SpriteRenderComponent>();
	}

	void EditorLayer::onDetatch()
	{

	}

	void EditorLayer::onImGuiRender()
	{
		static bool dockspaceOpen = true;
		ImGuiLib::createDockspace(dockspaceOpen, "Dockspace");

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					dockspaceOpen = false;
					App::get().close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		

		//ImGui::Begin("Data and stuff");
		//ImGui::Text("Scene hierarchy in here");
		//ImGui::End();

		sceneHierarchy.onImGuiRender();

		ImGui::Begin("Inspector");
		ImGui::DragFloat3("Camera Position", glm::value_ptr(testPosition));
		ImGui::ColorEdit4("Quad Colour", glm::value_ptr(testColour));
		ImGui::DragFloat2("Size", glm::value_ptr(testSize), 0.1f);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		viewportFocus = ImGui::IsWindowFocused();
		viewportHover = ImGui::IsWindowHovered();
		App::get().getImguiLayer()->blockEvents(!viewportFocus || viewportHover);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		viewportSize = {viewportPanelSize.x, viewportPanelSize.y};

		uint32_t textureID = framebuffer->getColorAttachmentID();
		ImGui::Image((ImTextureID)textureID, ImVec2{viewportSize.x, viewportSize.y}, 
			ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		
		//renderer3d->preProcessing();
		if (FramebufferSpecification spec = framebuffer->getSpecification();
			viewportSize.x > 0.0f && viewportSize.y > 0.0f &&
			(spec.w != viewportSize.x || spec.h != viewportSize.y))
		{
			framebuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			camera.OnResize(viewportSize.x, viewportSize.y);
			camera.SetViewportSize(viewportSize.x, viewportSize.y);
			activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		}

		Render2d::resetStats();
		framebuffer->bind();

		Render2d::setClearColour({ 0.2f, 0.2f, 0.2f, 1.0f });
		Render2d::clear();

		camera.onUpdate(ts);

		testCamera.getComponent<TransformComponent>().transform = testPosition;
		testEntity.getComponent<SpriteRenderComponent>().colour = testColour;
		testEntity.getComponent<TransformComponent>().scale = { testSize.x, testSize.y, 1.0f };

		activeScene->onUpdate(ts);

		auto stat = Render2d::getStats();

		
		//render2d->clear();
		//render2d->resetStats();

		//LOG_TRACE(glm::to_string(camera.GetPosition()) + " " + glm::to_string(camera.GetRightDirection()));


		//
		//render2d->beginScene(camera);

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

		//for (float c = -5; c < 5; c++)
		//{
		//	for (float r = -5; r < 5; r++)
		//	{
		//		float red = (c + 5.0f) / 10.0f;
		//		float green = (r + 5.0f) / 10.0f;
		//		//render2d->drawQuad(glm::vec3(c, r, 0.0f), glm::vec2(0.5f, 0.5f), { red, green, red + green, 1.0f });
		//		//render2d->drawQuad(glm::vec3(c, r, 0.0f), testSize, testColour);
		//	}
		//}
		//render2d->endScene();
		framebuffer->unbind();
		{
			//auto stats = render2d->getStats();
			//auto error = glGetError();
			//if (error) { LOG_ERROR("GL Error: {0}", error); }
			//LOG_INFO("Draw Calls: {0}", stats.drawCalls);
			//framebuffer->unbind();
		}
	}

	void EditorLayer::onEvent(Event& e)
	{
		camera.onEvent(e);
		
	}

}