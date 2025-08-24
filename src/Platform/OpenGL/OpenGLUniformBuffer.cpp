#include "GameEngine_PCH.h"
#include "OpenGLUniformBuffer.h"
#include <gl3w.h>

namespace GameEngine
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t s, uint32_t b)
	{
		glCreateBuffers(1, &id);
		glNamedBufferData(id, s, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, b, id);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(id, offset, size, data);
	}
}