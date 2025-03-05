#include "GameEngine_PCH.h"
#include "OpenGLFramebuffer.h"
#include <gl3w.h>
#include "OpenGLFramebufferUtils.h"

namespace GameEngine
{
	static const uint32_t maxSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& s) : spec(s)
	{
		namespace Utils = GameEngine::FramebufferUtils::OpenGLUtils;
		for (auto s : spec.attachments.attachments)
		{
			if (!Utils::isDepthFormat(s.textureFormat))
			{
				colourAttachSpecifications.emplace_back(s);
			}
			else depthAttachSpecification = s;
		}
		invalidate(); 
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() { cleanup(); }

	void OpenGLFramebuffer::cleanup()
	{
		glDeleteFramebuffers(1, &id);
		glDeleteTextures(colourAttachments.size(), colourAttachments.data());
		glDeleteTextures(1, &depthAttachment);

		colourAttachments.clear();
		depthAttachment = 0;
	}

	void OpenGLFramebuffer::invalidate()
	{
		namespace Utils = GameEngine::FramebufferUtils::OpenGLUtils;
		if (id)
		{
			cleanup();
		}
		
		glCreateFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		bool ms = spec.samples > 1;
		if (colourAttachSpecifications.size())
		{
			colourAttachments.resize(colourAttachSpecifications.size());
			Utils::createTextures(ms, colourAttachments.data(), colourAttachments.size());

			for (size_t i = 0; i < colourAttachments.size(); i++)
			{
				Utils::bindTexture(ms, colourAttachments[i]);
				switch (colourAttachSpecifications[i].textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::attachColourTexture(colourAttachments[i], spec.samples, GL_RGBA8, GL_RGBA, spec.w, spec.h, i);
					break;
				case FramebufferTextureFormat::RED_INT:
					Utils::attachColourTexture(colourAttachments[i], spec.samples, GL_R32I, GL_RED_INTEGER, spec.w, spec.h, i);
					break;
				}
			}
		}

		if (depthAttachSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			Utils::createTextures(ms, &depthAttachment, 1);
			Utils::bindTexture(ms, depthAttachment);

			switch (depthAttachSpecification.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::attachDepthTexture(depthAttachment, spec.samples, GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, spec.w, spec.h);
			}

		}

		if (colourAttachments.size() > 1)
		{
			CORE_ASSERT("Max colour attachemnts supported is 4", colourAttachments.size() <= 4);
			GLenum buffs[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
								GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(colourAttachments.size(), buffs);
		}
		else if (colourAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}


		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incorrectly formatted");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glViewport(0, 0, spec.w, spec.h);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t w, uint32_t h)
	{
		if (w == 0 || h == 0 || w > maxSize || h > maxSize)
		{
			LOG_WARN("Attempted to resize the framebuffer to {0}, {1}", w, h);
			return;
		}

		spec.w = w;
		spec.h = w;

		invalidate();
	}

	int OpenGLFramebuffer::readPixel(uint32_t idx, int x, int y)
	{
		CORE_ASSERT("attachment index exceeds number of colour attachments",
			idx < colourAttachments.size());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + idx);
		int pxData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pxData);
		return pxData;
	}

	void OpenGLFramebuffer::clearAttachment(uint32_t idx, int v)
	{
		namespace Utils = GameEngine::FramebufferUtils::OpenGLUtils;
		CORE_ASSERT("attachment index exceeds number of colour attachments",
			idx < colourAttachments.size());

		auto& spec = colourAttachSpecifications[idx];
		glClearTexImage(colourAttachments[idx], 0,
			Utils::textureFormatToGL(spec.textureFormat),
			GL_INT, &v);
	}
}