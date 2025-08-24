#include "GameEngine_PCH.h"
#include "Renderer2D.h"

#include "Renderer/Data/VertexArray.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Data/UniformBuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Render2DData.h"
#include <gl3w.h>

namespace GameEngine
{
	static Render2dData data;

	void Render2d::init()
	{
		data.quadArray = VertexArray::create();

		data.quadBuffer = VertexBuffer::create(data.maxVertices * sizeof(QuadVertex));
		data.quadBuffer->setLayout({
			{ShaderDataType::Float3, "position"},
			{ShaderDataType::Float4, "colour"},
			{ShaderDataType::Float2, "texCoord"},
			{ShaderDataType::Float2, "tilingFactor"},
			{ShaderDataType::Int, "entityID"}
			});

		data.quadArray->addVertexBuffer(data.quadBuffer);

		data.quadBufferBase = new QuadVertex[data.maxVertices];

		uint32_t* quadIndices = new uint32_t[data.maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, data.maxIndices);
		data.quadArray->setIndexBuffer(quadIB);
		delete[] quadIndices;

		data.circleArray = VertexArray::create();

		data.circleBuffer = VertexBuffer::create(data.maxVertices * sizeof(CircleVertex));
		data.circleBuffer->setLayout({
			{ShaderDataType::Float3, "worldPos"},
			{ShaderDataType::Float3, "localPos"},
			{ShaderDataType::Float4, "colour"},
			{ShaderDataType::Float, "thickness"},
			{ShaderDataType::Float, "fade"},
			{ShaderDataType::Int, "id"},
			});

		data.circleArray->addVertexBuffer(data.circleBuffer);
		data.circleArray->setIndexBuffer(quadIB);
		data.circleBufferBase = new CircleVertex[data.maxVertices];

		data.quadShader = createRef<Program>("assets/shaders/quad2d");
		data.circleShader = createRef<Program>("assets/shaders/circle2d");

		
		data.vertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		data.vertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
		data.vertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
		data.vertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

		data.cameraBuffer = UniformBuffer::create(sizeof(Render2dData::CameraData), 0);
		
		glEnable(GL_DEPTH_TEST);
	}

	void Render2d::shutdown()
	{
		delete[] data.quadBufferBase;
	}

	void Render2d::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Render2d::setClearColour(const glm::vec4& c)
	{
		glClearColor(c.r, c.g, c.b, c.a);
	}

	void Render2d::beginScene(const Camera& camera, const glm::mat4& transform)
	{
		data.camera.viewProjection = camera.GetProjection() * glm::inverse(transform);
		data.cameraBuffer->setData(&data.camera, sizeof(Render2dData::CameraData));

		startBatch();
	}

	void Render2d::beginScene(const EditorCamera& camera)
	{
		data.camera.viewProjection = camera.getViewProjection();
		data.cameraBuffer->setData(&data.camera, sizeof(Render2dData::CameraData));

		startBatch();
	}

	void Render2d::endScene()
	{
		flush();
	}

	void Render2d::startBatch()
	{
		data.quadIndexCount = 0;
		data.quadBufferPtr = data.quadBufferBase;
		//needs highlight on the github page for circle render component
		data.circleIndexCount = 0;
		data.circleBufferPtr = data.circleBufferBase;
	}

	void Render2d::flush()
	{
		if (data.quadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)data.quadBufferPtr - (uint8_t*)data.quadBufferBase);
			data.quadBuffer->setData(data.quadBufferBase, dataSize);

			data.quadShader->bind();

			data.quadArray->bind();

			uint32_t count = data.quadIndexCount ? data.quadIndexCount :
				data.quadArray->getIndexBuffer()->getCount();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			data.stats.drawCalls++;
		}

		if (data.circleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)data.circleBufferPtr - 
				(uint8_t*)data.circleBufferBase);
			data.circleBuffer->setData(data.circleBufferBase, dataSize);

			data.circleShader->bind();

			data.circleArray->bind();
			uint32_t count = data.circleIndexCount ? data.circleIndexCount : 
				data.circleArray->getIndexBuffer()->getCount();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
			
			data.stats.drawCalls++;
		}
	}

	void Render2d::nextBatch()
	{
		flush();
		startBatch();
	}

	void Render2d::drawQuad(const glm::vec2& p, const glm::vec2 s, const glm::vec4 c)
	{
		drawQuad({ p.x, p.y, 0.0f }, s, c);
	}

	void Render2d::drawQuad(const glm::vec3& p, const glm::vec2 s, const glm::vec4 c)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), p) *
			glm::scale(glm::mat4(1.0f), glm::vec3(s.x, s.y, 1.0f));

		drawQuad(transform, c);
	}

	void Render2d::drawQuad(const glm::mat4& t, const glm::vec4 c, int id)
	{
		constexpr size_t  vertCount = 4;
		constexpr glm::vec2 textureCoordinates[] =
		{
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};
		constexpr glm::vec2 tiling = { 1.0f, 1.0f };

		if (data.quadIndexCount >= Render2dData::maxIndices)
		{
			nextBatch();
		}

		for (size_t i = 0; i < vertCount; i++)
		{
			data.quadBufferPtr->pos = t * data.vertexPositions[i];
			data.quadBufferPtr->colour = c;
			data.quadBufferPtr->texCoord = textureCoordinates[i];
			data.quadBufferPtr->tiling = tiling;
			data.quadBufferPtr->entityID = id;
			data.quadBufferPtr++;
		}

		data.quadIndexCount += 6;
		data.stats.quadCount++;
	}

	void Render2d::drawRotatedQuad(const glm::vec2& p, const glm::vec2 s, const float rot, const glm::vec4 c)
	{
		drawRotatedQuad({ p.x, p.y, 0.0f }, s, rot, c);
	}

	void Render2d::drawRotatedQuad(const glm::vec3& p, const glm::vec2 s, const float rot, const glm::vec4 c)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), p)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rot), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { s.x, s.y, 1.0f });

		drawQuad(transform, c);
	}

	void Render2d::drawCircle(const glm::mat4& transform, const glm::vec4 colour, float thickness, float fade, int entityID)
	{
		if (data.circleIndexCount >= Render2dData::maxIndices)
		{
			nextBatch();
		}

		for (size_t i = 0; i < 4; i++)
		{
			data.circleBufferPtr->worldPos = transform * data.vertexPositions[i];
			data.circleBufferPtr->localPos = data.vertexPositions[i] * 2.0f;
			data.circleBufferPtr->colour = colour;
			data.circleBufferPtr->thickness = thickness;
			data.circleBufferPtr->fade = fade;
			data.circleBufferPtr->eID = entityID;
			data.circleBufferPtr++;
		}
		data.circleIndexCount += 6;
		data.stats.quadCount++;
	}

	void Render2d::drawSprite(const glm::mat4& transform, SpriteRenderComponent& src, int id)
	{
		drawQuad(transform, src.colour, id);
	}

	void Render2d::resetStats() { memset(&data.stats, 0, sizeof(Statistics)); }
	Render2d::Statistics Render2d::getStats() { return data.stats; }

}