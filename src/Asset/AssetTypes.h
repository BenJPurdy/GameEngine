#pragma once
#include "Core/Core.h"
#include "Logging/Log.h"
#include <string_view>

namespace GameEngine
{
	enum class AssetType : uint16_t
	{
		None = 0,
		Mesh,
		StaticMesh,
		MeshSource,
		Material,
		Texture,
		Audio
	};

	namespace Utils
	{
		inline AssetType assetTypeFromString(std::string_view t)
		{
			if (t == "None")		return AssetType::None;
			if (t == "Mesh")		return AssetType::Mesh;
			if (t == "StaticMesh")	return AssetType::StaticMesh;
			if (t == "MeshSource")	return AssetType::MeshSource;
			if (t == "Material")	return AssetType::Material;
			if (t == "Texture")		return AssetType::Texture;
			if (t == "Audio")		return AssetType::Audio;
			return AssetType::None;
		}

		inline const char* assetTypeToString(AssetType t)
		{
			switch (t)
			{
			case AssetType::None:		return "None";
			case AssetType::Mesh:		return "Mesh";
			case AssetType::StaticMesh: return "StaticMesh";
			case AssetType::MeshSource: return "MeshSource";
			case AssetType::Material:	return "Material";
			case AssetType::Texture:	return "Texture";
			case AssetType::Audio:		return "Audio";
			}

			CORE_ASSERT(false, "Unknown Asset Type");
			return "None";
		}
	}
}
