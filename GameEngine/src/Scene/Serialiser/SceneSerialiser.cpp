#include "GameEngine_PCH.h"
#include "SceneSerialiser.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "yamlConverter.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

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
			out << YAML::Key << "Primary" << YAML::Value << camComp.primary;
			out << YAML::Key << "FixedAspect" << YAML::Value << camComp.fixedAspect;

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
					auto& cc = deserialised.addComponent<CameraComponent>();

					auto& camProps = camComp["Camera"];
					cc.camera.setProjectionType((ProjectionType)camProps["ProjectionType"].as<int>());

					cc.camera.setPerspectiveFOV(camProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNear(camProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFar(camProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(camProps["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNear(camProps["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFar(camProps["OrthographicFar"].as<float>());

					cc.primary = camProps["Primary"].as<bool>();
					cc.fixedAspect = camProps["FixedAspect"].as<bool>();
				}

				auto SRC = e["SpriteRenderComponent"];
				if (SRC)
				{
					auto& src = deserialised.addComponent<SpriteRenderComponent>();
					src.colour = SRC["Colour"].as<glm::vec4>();
				}

				//add more components as they are added to the engine
			}
		}
		return true;
	}
}