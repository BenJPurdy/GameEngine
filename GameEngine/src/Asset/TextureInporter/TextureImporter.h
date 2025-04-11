#pragma once

#include "Renderer/Texture/Texture.h"
#include "Renderer/Texture/Texture2d.h"
#include "Asset/Databuffer.h"

#include <filesystem>

namespace GameEngine
{
	enum class ImageFormat
	{
		None = 0,
		RED8UN,
		RED8UI,
		RED16UI,
		RED32UI,
		RG8,
		RG16F,
		RG32F,
		RGB,
		RGBA,
		RGBA16F,
		RGBA32F,

		SRGB,
		SRGBA,

		DEPTH32FSTENCIL8UINT,
		DEPTH32F,
		DEPTH24STENCIL8,

		Depth = DEPTH24STENCIL8,
	};

	class TextureImporter
	{
	public: 
		static DataBuffer toBufferFromFile(const std::filesystem::path&, uint32_t&, uint32_t&);
		static DataBuffer toBufferFromMemory(DataBuffer, uint32_t&, uint32_t&);
	private:
		const std::filesystem::path path;
	};
}
