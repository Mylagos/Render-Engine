#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/matrix.hpp>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
		glm::mat4 trans_;
		glm::mat4 model_ = glm::mat4(1.0f);

		glm::vec3 cubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
	};

	void HelloSquare::Begin()
	{

		//!\\ LOAD TEXTURE
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

		//free image from memory
		stbi_image_free(data);

		//!\\ LOAD SHADER
		const auto vertexContent = LoadFile("data/shaders/hello_tutorial/3Dcube.vert");
		const auto* vertexShaderSource = vertexContent.data();

		const auto fragmentContent = LoadFile("data/shaders/hello_tutorial/3Dcube.frag");
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

		//!\\ CREATE PROGRAM

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
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		glUseProgram(program_);

		glUniform1i(glGetUniformLocation(program_, "texture1"), 0); // set it manually


		unsigned int transformLoc = glGetUniformLocation(program_, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans_));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	float dt2 = 0;
	double x = 1,
		y = 0,
		z = 0;
	void HelloSquare::Update(float dt)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glUseProgram(program_);
		glBindVertexArray(vao_);
		dt2 += 2;
		std::cout << dt2 << std::endl;

		trans_ = glm::mat4(1.0f);
		trans_ = glm::translate(trans_, glm::vec3(0.0f, 0.0f, 0.0f));
		trans_ = glm::rotate(trans_, glm::radians(dt2), glm::vec3(x, y, z));
		trans_ = glm::scale(trans_, glm::vec3(0.5, 0.5, 0.5));
		unsigned int transformLoc = glGetUniformLocation(program_, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans_));

		

		int projlUn = glGetUniformLocation(program_, "projection");
		glUniformMatrix4fv(projlUn, 1, GL_FALSE, glm::value_ptr(camProj_));

		int viewUn = glGetUniformLocation(program_, "view");
		glUniformMatrix4fv(viewUn, 1, GL_FALSE, glm::value_ptr(camView_));

		for (unsigned int i = 0; i < 10; i++)
		{
			model_ = glm::mat4(1.0f);
			model_ = glm::translate(model_, cubePositions[i]);
			float angle = 20.0f * i;
			model_ = glm::rotate(model_, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			int modelUn = glGetUniformLocation(program_, "model");
			glUniformMatrix4fv(modelUn, 1, GL_FALSE, glm::value_ptr(model_));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glDrawArrays(GL_TRIANGLES, 0, 36);
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

