#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
		int width, height, nrChannels;
		unsigned char* data = stbi_load("data/textures/zizou_cropped.jpg", &width, &height, &nrChannels, 0);

		unsigned int texture0;
		glGenTextures(1, &texture0);
		glBindTexture(GL_TEXTURE_2D, texture0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

		if (data)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		//FREE IMAGE FROM MEMORY
		stbi_image_free(data);

		unsigned int texture1;
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		data = stbi_load("data/textures/scooby_uchawi.jpg", &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);




		const auto vertexContent = LoadFile("data/shaders/hello_tutorial/texture.vert");
		const auto* vertexShaderSource = vertexContent.data();

		const auto fragmentContent = LoadFile("data/shaders/hello_tutorial/texture.frag");
		const auto* fragmentShaderSource = fragmentContent.data();

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

		float vertices[] = {
			0.5f,  0.5f, 0.0f,  // top right
			0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		unsigned int EBO;
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glUseProgram(program_);

		glUniform1i(glGetUniformLocation(program_, "texture1"), 0); // set it manually
		glUniform1i(glGetUniformLocation(program_, "texture2"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void HelloSquare::Update(float dt)
	{
		glUseProgram(program_);
		glBindVertexArray(vao_);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		int nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
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

