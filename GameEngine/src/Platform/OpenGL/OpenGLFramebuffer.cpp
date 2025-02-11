#include "GameEngine_PCH.h"
#include "OpenGLFramebuffer.h"
#include <gl3w.h>

namespace GameEngine
{
	static const uint32_t maxSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& s) : spec(s) { invalidate(); }

	OpenGLFramebuffer::~OpenGLFramebuffer() { cleanup(); }

	void OpenGLFramebuffer::cleanup()
	{
		glDeleteFramebuffers(1, &id);
		glDeleteTextures(1, &colour);
		glDeleteTextures(1, &depth);
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (id)
		{
			cleanup();
		}
		glCreateFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		glCreateTextures(GL_TEXTURE_2D, 1, &colour);
		glBindTexture(GL_TEXTURE_2D, colour);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.w, spec.h,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, colour, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &depth);
		glBindTexture(GL_TEXTURE_2D, depth);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec.w, spec.h);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_TEXTURE_2D, depth, 0);

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
}