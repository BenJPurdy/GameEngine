#pragma once

namespace GameEngine
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual uint32_t getCount() const = 0;

		static Ref<IndexBuffer> create(uint32_t*, uint32_t);
	};
}