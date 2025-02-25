#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine
{
	class ImGuiLib
	{
	public:
		static void createDockspace(bool& open, const char* name)
		{
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

			auto windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const auto* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			{
				windowFlags |= ImGuiWindowFlags_NoBackground;
			}

			if (!opt_padding)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			}
			ImGui::Begin(name, &open, windowFlags);
			if (!opt_padding)
			{
				ImGui::PopStyleVar();
			}
			if (opt_fullscreen)
			{
				ImGui::PopStyleVar(2);
			}

			auto& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}
		}

		template<typename UIFunction, typename... Args>
		static void drawMenuItem(const std::string& l, UIFunction uiFn, Args&&... args)
		{
			if (ImGui::MenuItem(l.c_str()))
			{
				uiFn(std::forward<Args>(args)...);
			}
		}

		template<typename UIFunction>
		static void drawCenteredButton(const std::string& l, float pcWidth, UIFunction uiFn)
		{
			ImVec2 availRegion = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			float buttonW = availRegion.x * pcWidth;
			float buttonXPos = (availRegion.x - buttonW) * 0.5f;

			ImGui::Columns(2, NULL, false);
			ImGui::SetColumnWidth(0, (availRegion.x - buttonW) * 0.5f);
			ImGui::NextColumn();

			if (ImGui::Button(l.c_str(), ImVec2{ buttonW, lineHeight }))
			{
				uiFn();
			}
			
			ImGui::NextColumn();
			ImGui::Columns(1);
		}

		template<typename T, typename UIFunction>
		static void drawComponent(const std::string& n, Entity e, UIFunction uiFn)
		{
			const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

			if (e.hasComponent<T>())
			{
				auto& component = e.getComponent<T>();
				ImVec2 availRegion = ImGui::GetContentRegionAvail();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				float lh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

				ImGui::Separator();

				bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeFlags, n.c_str());
				ImGui::PopStyleVar();
				ImGui::SameLine(availRegion.x - lh * 0.5f);
				if (ImGui::Button("+", ImVec2{ lh, lh }))
				{
					ImGui::OpenPopup("Component Setting");
				}
				bool removeComponent = false;
				if (ImGui::BeginPopup("Component Settings"))
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						removeComponent = true;
					}
					ImGui::EndPopup();
				}

				if (open)
				{
					uiFn(component);
					ImGui::TreePop();
				}

				if (removeComponent)
				{
					e.removeComponent<T>();
				}
			}
		}

		static void drawVec3Control(const std::string& l, glm::vec3& v,
			float resetValue = 0.0f, float columnWidth = 100.0f)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(l.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(l.c_str());

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lh + 3.0f, lh };

			
		}
	};
}
