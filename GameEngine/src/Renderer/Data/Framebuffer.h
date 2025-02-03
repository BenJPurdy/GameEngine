#pragma once
#include "Core/Core.h"

namespace GameEngine
{
	struct FramebufferSpecification
	{
		uint32_t w, h;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification&);
		~Framebuffer();

		void bind();
		void unbind();

		void invalidate();
		void cleanup();

		void resize(uint32_t, uint32_t);

		inline uint32_t getColorAttachmentID() const { return colourAttachment; }
		inline const FramebufferSpecification& getSpecification() const { return spec; }

	private:
		uint32_t id;
		uint32_t colourAttachment, depthAttachment;
		FramebufferSpecification spec;

	};
}