#include "GameEngine_PCH.h"
#include "Render3D.h"

namespace GameEngine
{
	void Render3d::preProcessing()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Render3d::setupTriangle()
	{
		//LOG_TRACE("Setting up a triangle");
		glGenVertexArrays(1, &triVAO);
		glBindVertexArray(triVAO);

		glGenBuffers(1, &triVBO);
		glBindBuffer(GL_ARRAY_BUFFER, triVBO);

		float verts[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &triIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triIBO);

		uint32_t idx[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

		glBindVertexArray(triVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triIBO);
	}

	void Render3d::renderTriangle()
	{
		glBindVertexArray(triVAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}
}