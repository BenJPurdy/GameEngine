#pragma once
#include "Renderer/Data/VertexBuffer.h"

namespace GameEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float*, uint32_t);
		OpenGLVertexBuffer(uint32_t);

		virtual ~OpenGLVertexBuffer();
		
		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setData(const void*, uint32_t) override;

		virtual const BufferLayout& getLayout() const override { return layout; }
		virtual void setLayout(const BufferLayout& l) override { layout = l; }

	private:
		uint32_t id;
		BufferLayout layout;
	};
}
