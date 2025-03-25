#include "GameEngine_PCH.h"

#include "EditorLayer.h"
#include "ImGui/ImGuiLib.h"

#include "ImGuizmo.h"
#include "Maths/Maths.h"

#include "Utilities/PlatformUtils.h"
#include "Scene/Serialiser/SceneSerialiser.h"

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
		spec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth };
		spec.w = 1280;
		spec.h = 720;
		framebuffer = Framebuffer::create(spec);

		editorScene = createRef<Scene>();
		activeScene = editorScene;
		sceneHierarchy.setContext(activeScene);

		//renderer3d = createRef<Render3d>();
		//renderer3d->setupTriangle();
		//
		//render2d = createRef<Render2d>();
		//render2d->init();
		//render2d->setClearColour({ 0.2f, 0.2f, 0.2f, 1.0f });

		shaders = createRef<ShaderLibrary>();
		shaders->load("flatColour", "assets/shaders/flatColour");

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
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					newScene();
				}
				if (ImGui::MenuItem("Open..", "Ctrl+O"))
				{
					openScene();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					saveScene();
				}
				if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
				{
					saveSceneAs();
				}
				//maybe change this from ctrl+q since Q is a bind for everyday functionality
				//with the gizmos
				if (ImGui::MenuItem("Exit", "Ctrl+Q"))
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


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		float toolbarHeight = 30.0f;

		ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetContentRegionAvail().x, toolbarHeight), true);
		ImVec2 buttonSize = { 50, toolbarHeight };

		if (ImGui::Button("Move", buttonSize))
		{
			if (!ImGuizmo::IsUsing())
			{
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Rotate", buttonSize))
		{
			if (!ImGuizmo::IsUsing())
			{
				gizmoType = ImGuizmo::OPERATION::ROTATE;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Scale", buttonSize))
		{
			if (!ImGuizmo::IsUsing())
			{
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			}
		}

		ImGui::NextColumn();
		ImGui::SameLine();
		char* buttonName = (sceneState == SceneState::Edit) ? "Play" : "Stop";
		if (ImGui::Button(buttonName, buttonSize))
		{
			if (sceneState == SceneState::Edit)
			{
				onScenePlay();
				
			}
			else if (sceneState == SceneState::Play)
			{
				onSceneStop();
				
			}
		}
		if (ImGui::Button("compile"))
		{
			// do system call
			// auto lib = LoadLibraryA(built lib)
			// functions are defined as extern "C" to stop c++ name mangling at compile time
			// populate map of fnptr with names + lib using GetProcAddress(lib, "fnName");
		}
		ImGui::EndChild();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		auto viewportOffset = ImGui::GetCursorScreenPos();
		viewportBounds[0] = { viewportOffset.x, viewportOffset.y };
		viewportBounds[1] = { viewportOffset.x + viewportPanelSize.x, viewportOffset.y + viewportPanelSize.y };

		viewportFocus = ImGui::IsWindowFocused();
		viewportHover = ImGui::IsWindowHovered();
		App::get().getImguiLayer()->blockEvents(!viewportFocus && !viewportHover);
		
		viewportSize = {viewportPanelSize.x, viewportPanelSize.y};

		uint32_t textureID = framebuffer->getColourAttachmentRendererID();
		ImGui::Image((ImTextureID)textureID, ImVec2{viewportSize.x, viewportSize.y}, 
			ImVec2{0, 1}, ImVec2{1, 0});

		Entity selected = sceneHierarchy.getSelectedEntity();
		if (selected && gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(viewportOffset.x, viewportOffset.y, viewportPanelSize.x, viewportPanelSize.y);

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			const glm::mat4& proj = camera.GetProjection();
			glm::mat4 view = camera.GetViewMatrix();

			auto& entityTransform = selected.getComponent<TransformComponent>();
			glm::mat4 transform = entityTransform.getTransform();
			bool snap = Input::isKeyPressed(Key::LeftShift);
			float snapValue = 0.5f;
			if (gizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), (ImGuizmo::OPERATION)gizmoType,
				ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 pos, rot, scl;
				Maths::decomposeTransform(transform, pos, rot, scl);

				entityTransform.transform = pos;
				entityTransform.scale = scl;

				glm::vec3 dRot = rot - entityTransform.rotation;

				entityTransform.rotation += dRot;
			}
		}

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

		framebuffer->clearAttachment(1, -1);

		switch (sceneState)
		{
		case SceneState::Edit:
			camera.onUpdate(ts);
			activeScene->onUpdateEditor(ts, camera);
			break;
		case SceneState::Play:
			activeScene->onUpdateRuntime(ts);
			break;
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= viewportBounds[0].x;
		my -= viewportBounds[0].y;

		glm::vec2 viewportWidth = viewportBounds[1] - viewportBounds[0];

		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 &&
			mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pxData = framebuffer->readPixel(1, mouseX, mouseY);
			hoveredEntity = pxData == -1 ? Entity() : Entity((entt::entity)pxData, activeScene.get());
		}

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

		EventDispatcher disp(e);
		disp.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
		disp.dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::onKeyPressed));
	}

	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent e)
	{
		if (e.getMouseButton() == Mouse::ButtonLeft)
		{
			if (viewportHover && !ImGuizmo::IsOver() && !Input::isKeyPressed(Key::LeftAlt))
			{
				sceneHierarchy.setSelectedEntity(hoveredEntity);
			}
		}
		return false;
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent e)
	{
		if (e.getRepeatCount() > 0)
			return false;
		
		bool control = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
		bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);
		switch (e.getKeyCode())
		{
		case Key::N:
		{
			if (control)
				newScene();
			break;
		}
		case Key::D:
		{
			if (!control && shift)
				onDuplicateEntity();
			break;
		}
		case Key::Delete:
		{
			onDeleteEntity();
			break;
		}

		//save/load stuff
		case Key::O:
		{
			if (control)
				openScene();
			break;
		}
		case Key::S:
		{
			if (control && shift)
				saveSceneAs();
			if (control && !shift)
				saveScene();
			break;
		}

		//gizmo stuff
		case Key::Q:
		{
			if (viewportHover)
			{
				gizmoType = -1;
			}
			break;
		}
		case Key::W:
		{
			if (!ImGuizmo::IsUsing() && viewportHover)
			{
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			}
			break;
		}
		case Key::E:
		{
			if (!ImGuizmo::IsUsing() && viewportHover)
			{
				gizmoType = ImGuizmo::OPERATION::SCALE;
			}
			break;
		}
		case Key::R:
		{
			if (!ImGuizmo::IsUsing() && viewportHover)
			{
				gizmoType = ImGuizmo::OPERATION::ROTATE;
			}
			break;
		}

		}
	}

	void EditorLayer::newScene()
	{
		editorScene = createRef<Scene>();
		activeScene = editorScene;
		activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchy.setContext(activeScene);
		editorSceneFilePath.clear();
	}

	void EditorLayer::openScene()
	{
		std::string path = FileDialogs::openFile("GameEngine Scene (*.gameengine\0*.gameengine\0");
		if (!path.empty())
		{
			openScene(path);
		}
	}

	void EditorLayer::openScene(const std::filesystem::path& path)
	{
		if (sceneState != SceneState::Edit)
		{
			onSceneStop();
		}

		if (path.extension().string() != ".gameengine")
		{
			LOG_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}
		editorSceneFilePath = path;

		Ref<Scene> newScene = createRef<Scene>();
		SceneSerialiser serialiser(newScene);
		if (serialiser.deserialise(path.string()))
		{
			editorScene = newScene;
			editorScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			sceneHierarchy.setContext(editorScene);

			activeScene = editorScene;
		}
	}

	void EditorLayer::saveScene()
	{
		if (!editorSceneFilePath.empty())
		{
			serialiseScene(editorScene, editorSceneFilePath);
		}
		else
		{
			saveSceneAs();
		}
	}

	void EditorLayer::saveSceneAs()
	{
		std::string path = FileDialogs::saveFile("GameEngine Scene (*.gameengine)\0*.gameengine\0");
		if (path.find(".gameengine") == std::string::npos)
		{
			path += ".gameengine";
		}
		if (!path.empty())
		{
			serialiseScene(editorScene, path);
		}
		editorSceneFilePath = path;
	}

	void EditorLayer::serialiseScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerialiser serialiser(scene);
		serialiser.serialise(path.string());
	}


	void EditorLayer::onScenePlay()
	{
		sceneState = SceneState::Play;
		runtimeScene = createRef<Scene>();
		Scene::copyTo(editorScene, runtimeScene);

		runtimeScene->onRuntimeStart();

		activeScene = runtimeScene;
		sceneHierarchy.setContext(activeScene);
	}

	void EditorLayer::onSceneStop()
	{
		sceneState = SceneState::Edit;

		runtimeScene->onRuntimeStop();
		runtimeScene = nullptr;
		activeScene = editorScene;
		sceneHierarchy.setContext(activeScene);
	}

	void EditorLayer::onDuplicateEntity()
	{
		if (sceneState != SceneState::Edit)
		{
			LOG_WARN("Entity duplication can only be done in edit mode");
			return;
		}
		if (sceneHierarchy.getSelectedEntity())
		{
			editorScene->duplicateEntity(sceneHierarchy.getSelectedEntity());
		}
	}

	void EditorLayer::onDeleteEntity()
	{
		if (sceneState != SceneState::Edit)
		{
			LOG_WARN("Entity duplication can only be done in edit mode");
			return;
		}
		if (sceneHierarchy.getSelectedEntity())
		{
			editorScene->destoryEntity(sceneHierarchy.getSelectedEntity());
			sceneHierarchy.setSelectedEntity({});
		}
	}
}