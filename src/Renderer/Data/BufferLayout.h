#pragma once
#include "Buffer.h"

namespace GameEngine
{
	class BufferLayout
	{
	public:
		BufferLayout()
		{
		}

		BufferLayout(std::initializer_list<BufferElement> _elements) : elements(_elements)
		{
			calculateOffsetAndStride();
		}

		inline uint32_t getStride() const { return stride; }

		inline const std::vector<BufferElement>& getElements() const { return elements; }
		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }

		std::vector<BufferElement>::const_iterator begin()  const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end()	const { return elements.end(); }


	private:
		void calculateOffsetAndStride()
		{
			uint32_t offset = 0;
			stride = 0;
			for (auto& e : elements)
			{
				e.offset = offset;
				offset += e.size;
				stride += e.size;
			}
		}

		std::vector<BufferElement> elements;
		uint32_t stride = 0;
	};
}
