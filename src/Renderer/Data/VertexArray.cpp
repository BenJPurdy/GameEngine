#include "GameEngine_PCH.h"
#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GameEngine
{
	Ref<VertexArray> VertexArray::create()
	{
		return createRef<OpenGLVertexArray>();
	}
}