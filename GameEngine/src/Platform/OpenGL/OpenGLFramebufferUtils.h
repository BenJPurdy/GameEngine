#pragma once
#include "Renderer/Data/Framebuffer.h"

#include "gl3w.h"

namespace GameEngine::FramebufferUtils::OpenGLUtils
{
	static GLenum textureTarget(bool ms)
	{
		return ms ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void createTextures(bool ms, uint32_t* id, uint32_t count)
	{
		glCreateTextures(textureTarget(ms), count, id);
	}

	static void bindTexture(bool ms, uint32_t id)
	{
		glBindTexture(textureTarget(ms), id);
	}

	static void attachColourTexture(
		uint32_t id, int samples,
		GLenum intFmt, GLenum fmt,
		uint32_t width, uint32_t height, int idx)
	{
		bool ms = samples > 1;
		if (ms)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, intFmt,
				width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, intFmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			  
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, textureTarget(ms), id, 0);
	}

	static void attachDepthTexture(uint32_t id, int samples, GLenum fmt,
		GLenum attachType, uint32_t width, uint32_t height)
	{
		bool ms = samples > 1;
		if (ms)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, fmt, width, height, false);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, fmt, width, height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, textureTarget(ms), id, 0);
	}

	static bool isDepthFormat(FramebufferTextureFormat fmt)
	{
		switch (fmt)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
		}
		return false;
	}

	static GLenum textureFormatToGL(FramebufferTextureFormat fmt)
	{
		switch (fmt)
		{
		case FramebufferTextureFormat::RGBA8:
			return GL_RGBA8;
		case FramebufferTextureFormat::RED_INT:
			return GL_RED_INTEGER;
		}

		return 0;
	}

	static GLenum GLDataType(FramebufferTextureFormat fmt)
	{
		switch (fmt)
		{
		case FramebufferTextureFormat::RGBA8:
			return GL_UNSIGNED_BYTE;
		case FramebufferTextureFormat::RED_INT:
			return GL_INT;
		}
		return 0;
	}
	
}