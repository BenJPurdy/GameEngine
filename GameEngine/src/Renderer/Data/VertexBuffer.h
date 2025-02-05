#pragma once

#include "BufferLayout.h"

namespace GameEngine
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout&) = 0;

		virtual void setData(const void*, uint32_t) = 0;

		static Ref<VertexBuffer> create(uint32_t);
		static Ref<VertexBuffer> create(float*, uint32_t);
	};
}