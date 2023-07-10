#pragma once
#include "shader.h"
#include "file_utility.h"

namespace gpr5300
{
	int CUBEID = 0;

	class Cube
	{
	private:
		GLuint vao_;
		int thisCubeId;

	public:
		void CreateCube();
		void CreateTexturedCube();
		void CreateNormalCube();
		void Use();
		void Unbind();
		Cube();
	};

	inline void Cube::CreateCube()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, thisCubeId);
		glBindVertexArray(thisCubeId);
	}

	inline void Cube::CreateTexturedCube()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedCubeVertices), TexturedCubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, thisCubeId);
		glBindVertexArray(thisCubeId);
	}

	inline void Cube::CreateNormalCube()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVerticesWithNormal), CubeVerticesWithNormal, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, thisCubeId);
		glBindVertexArray(thisCubeId);
	}

	inline void Cube::Use()
	{
		glBindVertexArray(vao_);
	}

	inline void Cube::Unbind()
	{
		glBindVertexArray(thisCubeId);
	}

	inline Cube::Cube()
	{
		thisCubeId = CUBEID;
		CUBEID += 1;
	}
}
