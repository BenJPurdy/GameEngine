#pragma once
#include "Core/Core.h"

namespace GameEngine
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		RED_INT,
		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat fmt)
			: textureFormat(fmt) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachSpecification
	{
		FramebufferAttachSpecification() = default;
		FramebufferAttachSpecification(
			std::initializer_list<FramebufferTextureSpecification> attach)
			:attachments(attach) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t w = 0, h = 0;
		uint32_t samples = 1;
		bool swapChainTarget = false;
		FramebufferAttachSpecification attachments;
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

		virtual int readPixel(uint32_t, int, int) = 0;
		virtual void clearAttachment(uint32_t, int) = 0;
		virtual uint32_t getColourAttachmentRendererID(uint32_t index = 0) const = 0;
		
		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Ref<Framebuffer> create(const FramebufferSpecification&);

	};
}