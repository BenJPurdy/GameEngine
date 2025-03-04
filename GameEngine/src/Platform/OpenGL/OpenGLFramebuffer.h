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

		virtual int readPixel(uint32_t, int, int) override;
		virtual void clearAttachment(uint32_t, int) override;
		virtual uint32_t getColourAttachmentRendererID(uint32_t index = 0) const override
		{
			CORE_ASSERT("index exceeds number of colour attachments", index < colourAttachments.size());
			return colourAttachments[index];
		}


		//virtual uint32_t getColorAttachmentID() const override { return colour; }
		virtual const FramebufferSpecification& getSpecification() const override { return spec; }

	private:
		uint32_t id;
		FramebufferSpecification spec;

		std::vector<FramebufferTextureSpecification> colourAttachSpecifications;
		FramebufferTextureSpecification depthAttachSpecification;

		std::vector<uint32_t> colourAttachments;
		uint32_t depthAttachment;

	};
}