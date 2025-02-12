#pragma once

#include "Renderer2D.h"

#include "Renderer/Data/VertexArray.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Data/UniformBuffer.h"

#include "Renderer/Data/Primatives/QuadVertex.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine
{
	struct Render2dData
	{
		static const uint32_t maxQuads = 20000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextures = 32;

		Ref<VertexArray> quadArray;
		Ref<VertexBuffer> quadBuffer;
		Ref<Program> quadShader;

		uint32_t indexCount = 0;
		QuadVertex* bufferBase = nullptr;
		QuadVertex* bufferPtr = nullptr;

		glm::vec4 vertexPositions[4];

		Render2d::Statistics stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};

		CameraData camera;
		Ref<UniformBuffer> cameraBuffer;
	};
}