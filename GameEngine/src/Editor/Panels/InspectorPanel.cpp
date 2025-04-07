#include "GameEngine_PCH.h"
#include "InspectorPanel.h"

#include "ImGui/ImGuiLib.h"
//#include "imgui/imgui_stdlib.h"

#include <ImGui/imgui.h>
#include "Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>

#include "Utilities/PlatformUtils.h"

#define MAX_BUFFER_SIZE 512

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
			drawAddComponentMenuItem<CircleRenderComponent>(e, "Circle Renderer");
			//add components here
			drawAddComponentMenuItem<ScriptComponent>(e, "Script Component");
			//drawAddComponentMenuItem<Rigidbody2dComponent>(e, "Rigidbody Component");
			drawAddComponentMenuItem<BoxCollider2dComponent>(e, "Box Collider Component");
			drawAddComponentMenuItem<CircleCollider2dComponent>(e, "Circle Collider Component");
			drawAddComponentMenuItem<AudioComponent>(e, "Audio Component");
			drawAddComponentMenuItem<NetworkComponent>(e, "Network Component");


			ImGui::EndPopup();
		}
	}

	void drawPhysicsComponentProperties()
	{
		//ImGui::DragFloat2("Offset", )
	}

	std::string rigidbodyType(Entity e)
	{
		if (!e.hasComponent<Rigidbody2dComponent>()) return "None";
		auto rb = e.getComponent<Rigidbody2dComponent>();
		if (rb.has(PhysicsProperties::PhysProps_Static))
			return "Static";
		else if (rb.has(PhysicsProperties::PhysProps_Dynamic)) 
			return "Dynamic";
		else if (rb.has(PhysicsProperties::PhysProps_Kinematic)) 
			return "Kinematic";
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
			if (label == "Script Component")
			{
				//generate scriptable compontent file with attached function				
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

		//ImGuiLib::drawComponent<ScriptComponent>("Script", e, [](auto& comp)
		//	{
		//		
		//	};

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

		ImGuiLib::drawComponent<CircleRenderComponent>("Circle Renderer", e,
			[](auto& c)
			{
				ImGui::ColorEdit4("Colour", glm::value_ptr(c.colour));
				ImGui::DragFloat("Thickness", &c.thickness, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &c.fade, 0.0025f, 0.0f, 1.0f);
			});

		
		ImGuiLib::drawComponent<BoxCollider2dComponent>("Box Collider", e, [&](auto& comp)
			{
				if (!e.hasComponent<Rigidbody2dComponent>())
				{
					LOG_WARN("Adding rigidbody to object");
					e.addComponent<Rigidbody2dComponent>();
				}
				if (ImGui::Button("Reset to default values")) comp.setDefaults();
				ImGui::DragFloat2("Offset", glm::value_ptr(comp.offset), 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Density", &comp.density, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &comp.friction, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &comp.restitution, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat2("Bounds", glm::value_ptr(comp.extents), 0.05f, 0.0f, 1.0f);
				if (ImGui::Checkbox("Is Sensor", &comp.isSensor)) {}
			});

		ImGuiLib::drawComponent<CircleCollider2dComponent>("Circle Collider", e, [&](auto& comp)
			{
				if (!e.hasComponent<Rigidbody2dComponent>())
				{
					LOG_WARN("Adding rigidbody to object");
					e.addComponent<Rigidbody2dComponent>();
				}
				if (ImGui::Button("Reset to default values")) comp.setDefaults();
				ImGui::DragFloat2("Offset", glm::value_ptr(comp.offset), 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Density", &comp.density, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &comp.friction, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &comp.restitution, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Radius", &comp.radius, 0.05f, 0.0f, 1.0f);
				if (ImGui::Checkbox("Is Sensor", &comp.isSensor)) {}
			});

		ImGuiLib::drawComponent<Rigidbody2dComponent>("Rigidbody", e, [&](auto& comp)
			{
				if (!e.hasComponent<BoxCollider2dComponent>() && !e.hasComponent<CircleCollider2dComponent>())
				{
					LOG_WARN("Cannot have a rigidbody component without a collider component");
					e.removeComponent<Rigidbody2dComponent>();
					return;
				}
				std::string rbType = "None";
				auto props = comp.properties;
				if (comp.has(PhysicsProperties::PhysProps_Static))
					rbType = "Static";
				else if (comp.has(PhysicsProperties::PhysProps_Dynamic))
					rbType = "Dynamic";
				else if (comp.has(PhysicsProperties::PhysProps_Kinematic))
					rbType = "Kinematic";
				//ImGui::PushItemWidth(50.0f);
				if (ImGui::BeginCombo("Type", rbType.c_str()))
				{
					bool selected = false;
					if (ImGui::Selectable("Static", &selected))
					{
						comp.properties &= 0xF8; //zero out all body type bits (0xF8 = 0b1111000)
						comp.properties |= PhysicsProperties::PhysProps_Static;
					}

					if (ImGui::Selectable("Dynamic", &selected))
					{
						comp.properties &= 0xF8;
						comp.properties |= PhysicsProperties::PhysProps_Dynamic;
					}

					if (ImGui::Selectable("Kinematic", &selected))
					{
						comp.properties &= 0xF8;
						comp.properties |= PhysicsProperties::PhysProps_Kinematic;
					}

					ImGui::EndCombo();
				}				

				//ImGui::PopItemWidth();

				//ImGui::SameLine();
				bool fixedRot = comp.has(PhysicsProperties::PhysProps_FixedRotation);
				bool tickboxRot = fixedRot; //this variable holds the state to allow ImGui to draw the tickbox correctly
				if (ImGui::Checkbox("Fixed Rotation", &tickboxRot))
				{
					if (fixedRot)
					{
						comp.properties &= ~PhysicsProperties::PhysProps_FixedRotation;
					}
					else
					{
						comp.properties |= PhysicsProperties::PhysProps_FixedRotation;
					}
				}
				
				ImGui::NewLine();
				ImGui::DragFloat("Mass", &comp.body.mass, 0.05f, 0.0f, 1.0f);
			});

		ImGuiLib::drawComponent<ScriptComponent>("Script Component", e, [&](auto& comp)
			{
				char scriptNameBuffer[MAX_BUFFER_SIZE] = { "" };
				
				if (comp.script.size() > 0)
				{
					strncpy_s(scriptNameBuffer, comp.script.c_str(), sizeof(scriptNameBuffer));
				}
				
				menuItemTextInput("Script Name Specifier", comp.script, scriptNameBuffer);

			});

		ImGuiLib::drawComponent<AudioComponent>("Audio Component", e, [](auto& comp)
			{
				ImGui::TextWrapped(comp.localPath.c_str());
				char audioPath[MAX_BUFFER_SIZE] = { "" };
				if (ImGui::Button("Load Sound"))
				{
					std::string filePath = FileDialogs::openFile("Audio File (*.wav\0*.wav\0");
					comp.path = filePath;
					size_t found = comp.path.find("assets");
					std::string path = comp.path.substr(found);
					comp.localPath = path;
				}
			});

		ImGuiLib::drawComponent<NetworkComponent>("Network Component", e, [](auto& comp)
			{
				ImGui::Checkbox("Is Local", &comp.local);
			});

	}

	void InspectorPanel::checkAndCopyBuffer(std::string& check, char* buff)
	{
		if (check.size() > 0)
		{
			//strncpy_s(buff, check.c_str(), 512 * sizeof(char));
			strncpy_s(buff, check.size(), check.c_str(), 512);
		}
	}

	void InspectorPanel::menuItemTextInput(const char* name, std::string& out, char* buff)
	{
		ImGui::Text(name);
		ImGui::PushItemWidth(-1);
		intptr_t nameP = reinterpret_cast<intptr_t>(name);
		std::string namePtr = std::to_string(nameP);
		std::string label = "##" + namePtr;
		
		if (ImGui::InputText(label.c_str(), buff, 512, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			out = buff;
		}
		ImGui::PopItemWidth();
	}
}