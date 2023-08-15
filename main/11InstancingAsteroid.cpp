#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/matrix.hpp>

#include "file_utility.h"
#include "engine.h"
#include "scene.h"
#include "cube.h"
#include "model.h"

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
		Shader myShader_;
		Shader planetShader_;
		float dt2 = 0;
		unsigned int asteroidAmount_ = 200000;
		glm::mat4* instancedModelMat;
		Model planetModel_;
		Model asteroidModel_;
		
	};

	void HelloSquare::Begin()
	{
		yaw_ = -72.0f;
		pitch_ = -25.0f;
		camPos_ = glm::vec3(-150.0f, 150.0f, 450.0f);

		myShader_ = Shader("data/shaders/adv_opengl/instancing.vert", "data/shaders/hello_lighting/Model.frag");
		myShader_.Use();
		planetShader_ = Shader("data/shaders/hello_lighting/Model.vert", "data/shaders/hello_lighting/Model.frag");

		planetModel_ = Model("data/model/planet/planet.obj");
		asteroidModel_ = Model("data/model/asteroid/rock.obj");

		
		instancedModelMat = new glm::mat4[asteroidAmount_];
		srand(time(NULL)); // initialize random seed
		float radius = 250.0;
		float offset = 50.0f;
		for (unsigned int i = 0; i < asteroidAmount_; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)asteroidAmount_ * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = static_cast<float>((rand() % 360));
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			instancedModelMat[i] = model;
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, asteroidAmount_ * sizeof(glm::mat4), &instancedModelMat[0], GL_STATIC_DRAW);
		for(unsigned int i = 0; i<asteroidModel_.meshes.size(); i++)
		{
			unsigned int VAO = asteroidModel_.meshes[i].VAO;
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}

	void HelloSquare::Update(float dt)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		dt2 += dt;
		if (dt2 > 6.3f)
		{
			dt2 -= 6.3f;
		}

		myShader_.Use();
		myShader_.SetProjViewMat(camProj_, camView_);

		planetShader_.Use();
		planetShader_.SetProjViewMat(camProj_, camView_);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		planetShader_.SetMat4("model", model);
		planetModel_.Draw(planetShader_);

		planetShader_.Use();
		planetShader_.SetInt("texture_diffuse1", 0);

		planetShader_.Use();
		glBindTexture(GL_TEXTURE_2D, asteroidModel_.textures_loaded[0].id);
		// MAKE THIS TRUE TO RENDER WITHOUT INSTANCING
		if (!true)
		{
			for (unsigned int i = 0; i < asteroidAmount_; i++)
			{
				planetShader_.SetMat4("model", instancedModelMat[i]);
				asteroidModel_.Draw(planetShader_);
				glBindVertexArray(0);
			}
		}

		myShader_.Use();
		myShader_.SetInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		// MAKE THIS TRUE TO RENDER WITH INSTANCING
		if (true)
		{
			for (unsigned int i = 0; i < asteroidModel_.meshes.size(); i++)
			{
				glBindVertexArray(asteroidModel_.meshes[i].VAO);
				glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(asteroidModel_.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, asteroidAmount_);
				glBindVertexArray(0);
			}
		}

	}

	void HelloSquare::End()
	{
		glDeleteProgram(myShader_.ID);
	}
}

int main(int argc, char** argv)
{

	gpr5300::HelloSquare scene;
	gpr5300::Engine engine(&scene);
	engine.Run();

	return EXIT_SUCCESS;
}

