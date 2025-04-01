#include "GameEngine_PCH.h"
#include "SceneSerialiser.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "yamlConverter.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

namespace GameEngine
{
	SceneSerialiser::SceneSerialiser(const Ref<Scene>& s) : scene(s) {}

	static void serialiseEntity(YAML::Emitter& out, Entity e)
	{
		CORE_ASSERT("Entity does not have ID Component", e.hasComponent<IDComponent>());

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << e.getUUID();

		if (e.hasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& t = e.getComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << t;
			out << YAML::EndMap;
		}

		if (e.hasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tr = e.getComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tr.transform;
			out << YAML::Key << "Rotation" << YAML::Value << tr.rotation;
			out << YAML::Key << "Scale" << YAML::Value << tr.scale;
			out << YAML::EndMap;
		}

		if (e.hasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			auto& camComp = e.getComponent<CameraComponent>();
			auto& cam = camComp.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)cam.getProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << cam.getPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << cam.getPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << cam.getPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << cam.getOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << cam.getOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << cam.getOrthographicFar();
			out << YAML::EndMap;
			out << YAML::Key << "Primary" << YAML::Value << (int)camComp.primary;
			out << YAML::Key << "FixedAspect" << YAML::Value << (int)camComp.fixedAspect;

			out << YAML::EndMap;
		}

		if (e.hasComponent<SpriteRenderComponent>())
		{
			out << YAML::Key << "SpriteRenderComponent";
			out << YAML::BeginMap;

			auto& SRC = e.getComponent<SpriteRenderComponent>();
			out << YAML::Key << "Colour" << YAML::Value << SRC.colour;
			out << YAML::EndMap;
		}

		if (e.hasComponent<CircleRenderComponent>())
		{
			out << YAML::Key << "CircleRenderComponent";
			out << YAML::BeginMap;
			auto& CRC = e.getComponent<CircleRenderComponent>();
			out << YAML::Key << "Colour" << YAML::Value << CRC.colour;
			out << YAML::Key << "Thickness" << YAML::Value<< CRC.thickness;
			out << YAML::Key << "Fade" << YAML::Value << CRC.fade;
			out << YAML::EndMap;
		}

		if (e.hasComponent<ScriptComponent>())
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;
			auto& scriptComp = e.getComponent<ScriptComponent>();
			out << YAML::Key << "Script";
			out << YAML::BeginMap;
			out << YAML::Key << "ScriptClass" << YAML::Value << scriptComp.script;
			out << YAML::Key << "OnStart" << YAML::Value << scriptComp.onStart;
			out << YAML::Key << "OnUpdate" << YAML::Value << scriptComp.onUpdate;
			out << YAML::Key << "OnCollisionStart" << YAML::Value << scriptComp.onCollisionEnter;
			out << YAML::Key << "OnCollisionExit" << YAML::Value << scriptComp.onCollisionExit;
			out << YAML::Key << "OnDestroy" << YAML::Value << scriptComp.onDestory;
			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (e.hasComponent<Rigidbody2dComponent>())
		{
			out << YAML::Key << "Rigidbody2dComponent";
			out << YAML::BeginMap;
			auto& rbc = e.getComponent<Rigidbody2dComponent>();
			out << YAML::Key << "Properties" << YAML::Value << rbc.properties;
			out << YAML::Key << "Mass" << YAML::Value << rbc.body.mass;
			out << YAML::EndMap;
		}

		if (e.hasComponent<CircleCollider2dComponent>())
		{
			out << YAML::Key << "CircleCollider2dComponent";
			out << YAML::BeginMap;
			auto& ccc = e.getComponent<CircleCollider2dComponent>();
			out << YAML::Key << "Radius" << YAML::Value << ccc.radius;
			out << YAML::Key << "Density" << YAML::Value << ccc.density;
			out << YAML::Key << "Friction" << YAML::Value << ccc.friction;
			out << YAML::Key << "Restitution" << YAML::Value << ccc.restitution;
			out << YAML::Key << "IsSensor" << YAML::Value << (int)ccc.isSensor;
			out << YAML::Key << "Offset" << YAML::Value << ccc.offset;
			out << YAML::EndMap;
		}

		if (e.hasComponent<BoxCollider2dComponent>())
		{
			out << YAML::Key << "BoxCollider2dComponent";
			out << YAML::BeginMap;
			auto& bcc = e.getComponent<BoxCollider2dComponent>();
			out << YAML::Key << "Extents" << YAML::Value << bcc.extents;
			out << YAML::Key << "Density" << YAML::Value << bcc.density;
			out << YAML::Key << "Friction" << YAML::Value << bcc.friction;
			out << YAML::Key << "Restitution" << YAML::Value << bcc.restitution;
			out << YAML::Key << "IsSensor" << YAML::Value << (int)bcc.isSensor;
			out << YAML::Key << "Offset" << YAML::Value << bcc.offset;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerialiser::serialise(const std::string& fp)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Untitled Scene";

		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		auto allE = scene->getRegistry().view<entt::entity>();
		for (auto eID : allE)
		{
			Entity e = { eID, scene.get() };
			if (!e) return;
			serialiseEntity(out, e);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(fp);
		fout << out.c_str();
	}

	bool SceneSerialiser::deserialise(const std::string& fp)
	{
		std::ifstream stream(fp);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data;

		try
		{
			data = YAML::Load(strStream.str());
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		if (!data["Scene"]) return false;

		std::string sceneName = data["Scene"].as<std::string>();

		LOG_TRACE("Deserialising Scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto e : entities)
			{
				uint64_t uuid = e["Entity"].as<uint64_t>();

				std::string name;
				auto tC = e["TagComponent"];
				if (tC)
				{
					name = tC["Tag"].as<std::string>();
				}
				LOG_TRACE("Deserialised entity with ID = {0}, name = {1}", uuid, name);

				Entity deserialised = scene->createEntityWithUUID(uuid, name);
				auto trC = e["TransformComponent"];
				if (trC)
				{
					auto& tC = deserialised.getComponent<TransformComponent>();
					tC.transform = trC["Translation"].as<glm::vec3>();
					tC.rotation = trC["Rotation"].as<glm::vec3>();
					tC.scale = trC["Scale"].as<glm::vec3>();
				}

				auto camComp = e["CameraComponent"];
				if (camComp)
				{
					LOG_TRACE("Loading Camera Component");
					auto& cc = deserialised.addComponent<CameraComponent>();

					auto& camProps = camComp["Camera"];
					cc.camera.setProjectionType((ProjectionType)camProps["ProjectionType"].as<int>());

					cc.camera.setPerspectiveFOV(camProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNear(camProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFar(camProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(camProps["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNear(camProps["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFar(camProps["OrthographicFar"].as<float>());

					cc.primary = (bool)camComp["Primary"].as<int>();
					cc.fixedAspect = (bool)camComp["FixedAspect"].as<int>();
				}

				auto SRC = e["SpriteRenderComponent"];
				if (SRC)
				{
					auto& src = deserialised.addComponent<SpriteRenderComponent>();
					src.colour = glm::make_vec4(SRC["Colour"].as<std::vector<float>>().data());
				}

				auto CRC = e["CircleRenderComponent"];
				if (CRC)
				{
					auto& crc = deserialised.addComponent<CircleRenderComponent>();
					crc.colour = glm::make_vec4(CRC["Colour"].as<std::vector<float>>().data());
					crc.thickness = CRC["Thickness"].as<float>();
					crc.fade = CRC["Fade"].as<float>();
				}

				auto RBC = e["Rigidbody2dComponent"];
				if (RBC)
				{
					auto& rbc = deserialised.addComponent<Rigidbody2dComponent>();
					rbc.set(RBC["Properties"].as<uint16_t>());
					rbc.body.mass = RBC["Mass"].as<float>();
				}

				auto CCC = e["CircleCollider2dComponent"];
				if (CCC)
				{
					auto& ccc = deserialised.addComponent<CircleCollider2dComponent>();
					ccc.radius = CCC["Radius"].as<float>();
					ccc.density = CCC["Density"].as<float>();
					ccc.friction = CCC["Friction"].as<float>();
					ccc.restitution = CCC["Restitution"].as<float>();
					ccc.offset = glm::make_vec2(CCC["Offset"].as<std::vector<float>>().data());
				}

				auto BCC = e["BoxCollider2dComponent"];
				if (BCC)
				{
					auto& bcc = deserialised.addComponent<BoxCollider2dComponent>();
					bcc.extents = glm::make_vec2(BCC["Extents"].as<std::vector<float>>().data());
					bcc.density = BCC["Density"].as<float>();
					bcc.friction = BCC["Friction"].as<float>();
					bcc.restitution = BCC["Restitution"].as<float>();
					bcc.offset = glm::make_vec2(BCC["Offset"].as<std::vector<float>>().data());
				}

				auto scriptComp = e["ScriptComponent"];
				if (scriptComp)
				{
					auto scripts = scriptComp["Script"];
					if (scripts)
					{

						auto& sC = deserialised.addComponent<ScriptComponent>();
						if (scripts["OnStart"]) { LOG_WARN("Found start fn"); }
						else { LOG_ERROR("Didn't find start fn"); }
						if (scripts["ScriptClass"]) sC.script = scripts["ScriptClass"].as<std::string>();
						sC.onStart = scripts["OnStart"].as<std::string>();
						sC.onUpdate = scripts["OnUpdate"].as<std::string>();
						sC.onCollisionEnter = scripts["OnCollisionStart"].as<std::string>();
						sC.onCollisionExit = scripts["OnCollisionExit"].as<std::string>();
						sC.onDestory = scripts["OnDestroy"].as<std::string>();
					}
					else
					{
						LOG_ERROR("Didn't find scripts");
					}
					
				}

				//add more components as they are added to the engine
			}
		}
		return true;
	}
}