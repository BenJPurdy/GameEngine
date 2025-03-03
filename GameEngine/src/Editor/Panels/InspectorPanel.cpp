#include "GameEngine_PCH.h"
#include "InspectorPanel.h"

#include "ImGui/ImGuiLib.h"

#include <ImGui/imgui.h>
#include "Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine
{
	void InspectorPanel::drawAddComponents(Entity e)
	{
		ImGui::Separator();
		ImGuiLib::drawCenteredButton("Add Component", 0.9f, []()
			{ImGui::OpenPopup("AddComponent"); });

		if (ImGui::BeginPopup("AddComponent"))
		{
			drawAddComponentMenuItem<SpriteRenderComponent>(e, "Sprite Renderer");
			drawAddComponentMenuItem<CameraComponent>(e, "Camera");

			ImGui::EndPopup();
		}
	}

	template<typename T>
	void InspectorPanel::drawAddComponentMenuItem(Entity e, const char* label)
	{
		if (!e.hasComponent<T>())
		{
			if (ImGui::MenuItem(label))
			{
				e.addComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void InspectorPanel::drawComponents(Entity e)
	{
		if (e.hasComponent<TagComponent>())
		{
			auto& t = e.getComponent<TagComponent>().tag;
			char buff[256];
			memset(buff, 0, sizeof(buff));
			strcpy_s(buff, sizeof(buff), t.c_str());
			if (ImGui::InputText("##Tag", buff, sizeof(buff)))
			{
				t = std::string(buff);
			}
		}

		ImGuiLib::drawComponent<TransformComponent>("Transform", e, [](auto& component)
			{
				ImGuiLib::drawVec3Control("Translation", component.transform);
				glm::vec3 rot = glm::degrees(component.rotation);
				ImGuiLib::drawVec3Control("Rotation", rot);
				component.rotation = glm::radians(rot);
				ImGuiLib::drawVec3Control("Scale", component.scale, 1.0f);
			});

		ImGuiLib::drawComponent<CameraComponent>("Camera", e, [](auto& component)
			{
				auto& cam = component.camera;

				const char* projectionTypes[] = { "Orthographic", "Perspective" };
				const char* currProjection = projectionTypes[(int)cam.getProjectionType()];
				if (ImGui::BeginCombo("Camera Type", currProjection))
				{
					for (int i = 0; i < 2; i++)
					{
						bool selected = currProjection == projectionTypes[i];
						if (ImGui::Selectable(projectionTypes[i], selected))
						{
							currProjection = projectionTypes[i];
							cam.setProjectionType((ProjectionType)i);
						}
						if (selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (cam.getProjectionType() == ProjectionType::Orthographic)
				{
					float orthoSize = cam.getOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize, 0.1f))
					{
						cam.setOrthographicSize(orthoSize);
					}

					float orthoNear = cam.getOrthographicNear();
					if (ImGui::DragFloat("Near", &orthoNear, 0.1f))
					{
						cam.setOrthographicNear(orthoNear);
					}

					float orthoFar = cam.getOrthographicFar();
					if (ImGui::DragFloat("Far", &orthoFar, 0.1f))
					{
						cam.setOrthographicFar(orthoFar);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspect);
				}

				if (cam.getProjectionType() == ProjectionType::Perspective)
				{
					float FOV = glm::degrees(cam.getPerspectiveFOV());
					if (ImGui::DragFloat("Size", &FOV, 0.1f))
					{
						cam.setPerspectiveFOV(glm::radians(FOV));
					}

					float perspNear = cam.getPerspectiveNear();
					if (ImGui::DragFloat("Near", &perspNear, 0.1f))
					{
						cam.setPerspectiveFar(perspNear);
					}

					float perspFar = cam.getPerspectiveFar();
					if (ImGui::DragFloat("Far", &perspFar, 0.1f))
					{
						cam.setPerspectiveFar(perspFar);
					}
				}
			});

		ImGuiLib::drawComponent<SpriteRenderComponent>("Sprite Renderer", e, [](auto& component)
			{
				//TODO Texturing and tiling fields
				ImGui::ColorEdit4("Colour", glm::value_ptr(component.colour));
			});

	}


}