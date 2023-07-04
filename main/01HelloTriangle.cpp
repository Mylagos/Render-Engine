#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

namespace gpr5300
{
	class HelloSquare final : public Scene
	{
	public:
		void Begin() override;
		void Update(float dt) override;
		void End() override;
	private:
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;
		GLuint program_ = 0;
		GLuint vao_ = 0;
	};

	void HelloSquare::Begin()
	{
		const auto vertexContent = LoadFile("data/shaders/hello_tutorial/triangle.vert");
		const auto* vertexShaderSource = vertexContent.data();

		const auto fragmentContent = LoadFile("data/shaders/hello_tutorial/triangle.frag");
		const auto* fragmentShaderSource = fragmentContent.data();

		constexpr float verticies[] = {
			-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
		};

		vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader_, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader_);

		GLint  success;
		char infoLog[512];
		glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader_, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader_);

		glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		program_ = glCreateProgram();

		glAttachShader(program_, vertexShader_);
		glAttachShader(program_, fragmentShader_);
		glLinkProgram(program_);

		glGetProgramiv(program_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program_, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader_);
		glDeleteShader(fragmentShader_);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void HelloSquare::Update(float dt)
	{
		glUseProgram(program_);
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void HelloSquare::End()
	{
		glDeleteProgram(program_);

		glDeleteShader(vertexShader_);
		glDeleteShader(fragmentShader_);

		glDeleteVertexArrays(1, &vao_);
	}
}

int main(int argc, char** argv)
{
	gpr5300::HelloSquare scene_;
	gpr5300::Engine engine(&scene_);
	engine.Run();

	return EXIT_SUCCESS;
}

