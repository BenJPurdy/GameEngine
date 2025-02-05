#pragma once
#include "Core/Core.h"

namespace GameEngine
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {};
		virtual void setData(const void*, uint32_t, uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> create(uint32_t, uint32_t);
	};
}