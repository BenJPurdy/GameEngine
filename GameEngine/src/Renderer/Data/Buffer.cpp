#include "GameEngine_PCH.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace GameEngine
{
	Ref<VertexBuffer> VertexBuffer::create(uint32_t s)
	{
		return createRef<OpenGLVertexBuffer>(s);
	}

	Ref<VertexBuffer> VertexBuffer::create(float* v, uint32_t s)
	{
		return createRef<OpenGLVertexBuffer>(v, s);
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* i, uint32_t s)
	{
		return createRef<OpenGLIndexBuffer>(i, s);
	}
}