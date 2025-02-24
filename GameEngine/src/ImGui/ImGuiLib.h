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
			if (ImGui::MenuItem(lable.c_str()))
			{
				uiFn(std::forward<Args>(args)...);
			}
		}
	};
}
