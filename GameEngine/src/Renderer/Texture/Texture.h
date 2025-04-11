#pragma once
#include <string>
#include "Core/Core.h"

namespace GameEngine
{
	class Texture
	{
	public:
		virtual ~Texture() {};
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getRenderID() const = 0;

		virtual void setData(void*, uint32_t) = 0;

		virtual void bind(uint32_t) const = 0;
		virtual bool operator==(const Texture& other) const = 0;
		virtual const std::string getPath() const = 0;
		virtual const std::string getType() const = 0;
		virtual void setType(std::string) = 0;

	};
}
