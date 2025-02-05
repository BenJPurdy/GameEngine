#include "GameEngine_PCH.h"
#include "OpenGLIndexBuffer.h"
#include <gl3w.h>

namespace GameEngine
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* i, uint32_t c)
	{
		glCreateBuffers(1, &id);

		glBindBuffer(GL_ARRAY_BUFFER, id);

		glBufferData(GL_ARRAY_BUFFER, c * sizeof(uint32_t), i, GL_STATIC_DRAW);
	}
	
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}