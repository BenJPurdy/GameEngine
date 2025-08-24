#pragma once
#include "Renderer/Data/VertexArray.h"

namespace GameEngine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const Ref<VertexBuffer>&) override;
		virtual void setIndexBuffer(const Ref<IndexBuffer>&) override;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override { return vertexBuffers; }
		virtual const Ref<IndexBuffer>& getIndexBuffer() const override { return indexBuffer; }

	private:
		uint32_t id = 0;
		uint32_t vertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
	};
}