#pragma once

#include <memory>
#include "Buffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace GameEngine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void addVertexBuffer(const Ref<VertexBuffer>&) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>&) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

		static Ref<VertexArray> create();
	};
}