#include "GameEngine_PCH.h"
#include "Texture2d.h"   
#include "stb/stb_image.h"

namespace GameEngine
{
	Texture2d::Texture2d(uint32_t _w, uint32_t _h) :width(_w), height(_h)
	{
		intFmt = GL_RGBA8;
		dataFmt = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &renderID);
		glTextureStorage2D(renderID, 1, intFmt, width, height);

		glTextureParameteri(renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(renderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2d::Texture2d(std::string p) :path(p)
	{
		int h, w, c;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &w, &h, &c, 0);
		}
		CORE_ASSERT(data, "Failed to load texture");
		width = w; height = h;
		GLenum fInt = 0, fDat = 0;
		if (c == 4)
		{
			fInt = GL_RGBA8;
			fDat = GL_RGBA;
		}
		else if (c == 3)
		{
			fInt = GL_RGB8;
			fDat = GL_RGB;
		}

		intFmt = fInt;
		dataFmt = fDat;

		CORE_ASSERT(fInt & fDat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &renderID);
		glTextureStorage2D(renderID, 1, intFmt, width, height);

		glTextureParameteri(renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(renderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(renderID, 0, 0, 0, width, height, fDat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	Texture2d::~Texture2d()
	{
		glDeleteTextures(1, &renderID);
	}

	void Texture2d::setData(void* d, uint32_t s)
	{
		uint32_t bpp = dataFmt == GL_RGBA ? 4 : 3;
		CORE_ASSERT(s == width * height * bpp, "Data must be for entire image");
		glTextureSubImage2D(renderID, 0, 0, 0, width, height, dataFmt, GL_UNSIGNED_BYTE, d);
	}

	void Texture2d::bind(uint32_t s) const
	{
		glBindTextureUnit(s, renderID);
	}
}
