#pragma once

#include "Core/Core.h"

namespace GameEngine
{
	struct DataBuffer
	{
		void* data = nullptr;
		uint64_t size = 0;

		DataBuffer() = default;
		DataBuffer(const void* d, uint64_t s = 0) : data((void*)d), size(s) {}
		static DataBuffer copy(const DataBuffer& other)
		{
			DataBuffer b;
			b.allocate(other.size);
			memcpy(b.data, other.data, other.size);
			return b;
		}

		static DataBuffer copy(const void* data, uint64_t size)
		{
			DataBuffer b;
			b.allocate(size);
			memcpy(b.data, data, size);
			return b;
		}

		void allocate(uint64_t s)
		{
			delete[](uint8_t*)data;
			data = nullptr;
			size = s;
			if (size == 0)
				return;
			data = new uint8_t[size];
		}

		void release()
		{
			delete[](uint8_t*)data;
			data = nullptr;
			size = 0;
		}

		void zeroInit()
		{
			if (data)
				memset(data, 0, size);
		}

		template<typename T>
		T& read(uint64_t offset = 0)
		{
			return *(T*)((byte*)data + offset);
		}

		template<typename T>
		const T& read(uint64_t offset = 0) const
		{
			return *(T*)((byte*)data + offset);
		}

		uint8_t* readBytes(uint64_t size, uint64_t offset) const
		{

		}
	};
}