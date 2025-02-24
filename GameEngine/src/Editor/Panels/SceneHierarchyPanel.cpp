#include "GameEngine_PCH.h"
#include "SceneHierarchyPanel.h"
#include "ImGui/ImGuiLib.h"
#include <imgui.h>
#include "Scene/Components.h"

namespace GameEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene>& ctx)
	{
		setContext(ctx);
	}

	void SceneHierarchyPanel::setContext(const Ref<Scene>& ctx)
	{
		context = ctx;
		selectedContext = {};
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (context)
		{
			auto view = context->getRegistry().view<entt::entity>();
			for (auto eID : view)
			{
				Entity e{ eID, context.get() };
				drawEntityNode(e);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				selectedContext = {};
			}

			rightClickMenu();
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		if (selectedContext)
		{
			//TODO
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::setSelectedEntity(Entity e)
	{
		selectedContext = e;
	}

	void SceneHierarchyPanel::drawEntityNode(Entity e)
	{
		auto& tag = e.getComponent<TagComponent>().tag;
		ImGuiTreeNodeFlags flags = ((selectedContext == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			selectedContext = e;
		}

		bool deleted = false;
		if (ImGui::BeginPopupContextItem("Delete Entity"))
		{
			//could add extra menu items here maybe????
			if (ImGui::MenuItem("Delete Entity"))
			{
				deleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}
		
		if (deleted)
		{
			context->destoryEntity(e);
			if (selectedContext == e)
			{
				selectedContext = {};
			}
		}
	}

	void SceneHierarchyPanel::rightClickMenu()
	{
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems));
		{
			ImGuiLib::drawMenuItem("Create Entity", [](Ref<Scene> ctx) { ctx->createEntity("Empty"); }, context);

			ImGuiLib::drawMenuItem("Camera", );

			ImGui::EndPopup();
		}
	}
}