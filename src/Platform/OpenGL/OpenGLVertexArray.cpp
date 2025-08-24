#include "GameEngine_PCH.h"
#include "OpenGLVertexArray.h"

#include <gl3w.h>

namespace GameEngine
{
	static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType t)
	{
		switch (t)
		{
		case ShaderDataType::Float:  //falling though cases 
		case ShaderDataType::Float2: 
		case ShaderDataType::Float3: 
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3: 
		case ShaderDataType::Mat4:  return GL_FLOAT;
		case ShaderDataType::Int: 
		case ShaderDataType::Int2: 
		case ShaderDataType::Int3: 
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		}
		CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &id);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(id);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& buffer)
	{
		CORE_ASSERT(buffer->getLayout().getElements().size(), "Vertex Buffer has no layout");
		glBindVertexArray(id);
		buffer->bind();
		const auto& layout = buffer->getLayout();
		
		for (const auto& e : layout)
		{
			switch (e.type)
			{
			case ShaderDataType::Float:  //falling though cases 
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribPointer(
					vertexBufferIndex,
					e.getComponentCount(),
					shaderDataTypeToOpenGLBaseType(e.type),
					e.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*)e.offset);
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = e.getComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribPointer(vertexBufferIndex,
						count, shaderDataTypeToOpenGLBaseType(e.type),
						e.normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)(e.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(vertexBufferIndex, 1);
					vertexBufferIndex++;
				}
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribIPointer(vertexBufferIndex,
					e.getComponentCount(),
					shaderDataTypeToOpenGLBaseType(e.type),
					layout.getStride(),
					(const void*)e.offset);
				vertexBufferIndex++;
				break;
			}
			}
		}
		vertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& idxBuffer)
	{
		glBindVertexArray(id);
		idxBuffer->bind();
		indexBuffer = idxBuffer;
	}
}