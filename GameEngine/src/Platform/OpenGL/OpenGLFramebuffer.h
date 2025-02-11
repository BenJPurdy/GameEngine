#pragma once
#include "Renderer/Data/Framebuffer.h"

namespace GameEngine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification&);
		virtual ~OpenGLFramebuffer();

		virtual void bind() override;
		virtual void unbind() override;
		virtual void invalidate() override;
		virtual void cleanup() override;
		virtual void resize(uint32_t, uint32_t) override;

		virtual uint32_t getColorAttachmentID() const override { return colour; }
		virtual const FramebufferSpecification& getSpecification() const override { return spec; }

	private:
		uint32_t id;
		uint32_t colour = 0;
		uint32_t depth = 0;
		FramebufferSpecification spec;
	};
}