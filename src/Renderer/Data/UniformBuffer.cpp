#include "GameEngine_PCH.h"
#include "UniformBuffer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace GameEngine
{
	Ref<UniformBuffer> UniformBuffer::create(uint32_t s, uint32_t b)
	{
		return createRef<OpenGLUniformBuffer>(s, b);
	}
}