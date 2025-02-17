#pragma once
#include "Core/Core.h"

namespace GameEngine
{
	struct FramebufferSpecification
	{
		uint32_t w = 0, h = 0;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void invalidate() = 0;
		virtual void cleanup() = 0;
		
		virtual void resize(uint32_t, uint32_t) = 0;
		
		virtual uint32_t getColorAttachmentID() const = 0;
		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Ref<Framebuffer> create(const FramebufferSpecification&);

	};
}