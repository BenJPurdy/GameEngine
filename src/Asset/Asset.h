#pragma once
#include "Core/UUID.h"
#include "Asset/AssetTypes.h"
#include <filesystem>

namespace GameEngine
{
	using AssetHandle = UUID;
	class Asset
	{
	public:
		AssetHandle handle = 0;
		virtual ~Asset() {}
		static AssetType getStaticType() { return AssetType::None; }
		virtual AssetType getAssetType() const { return AssetType::None; }
		virtual bool operator==(const Asset& other) const
		{
			return handle == other.handle;
		}
		virtual bool operator!= (const Asset& other) const
		{
			return !(*this == other);
		}
	private:
		friend class AssimpMeshImporter;
		friend class AssetManager;
	};

	struct AssetMetadata
	{
		AssetHandle handle = 0;
		AssetType type;
		std::filesystem::path path;
		std::string name;
		std::string ext;
		bool isValid() const { return handle != 0; }
		bool isDataLoaded = false;
	};
}
