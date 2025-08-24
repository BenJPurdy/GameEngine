#pragma once
#include "Renderer/Data/IndexBuffer.h"

namespace GameEngine
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t*, uint32_t);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const;
		virtual void unbind() const;

		virtual uint32_t getCount() const { return count; }
	private:
		uint32_t id;
		uint32_t count;
	};
}