#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <FileSystem>

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
		glm::mat4 trans_;
		glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 lightModel_ = glm::mat4(1.0f);
		glm::vec3 lightPos_ = glm::vec3(1.0f, 1.0f, 0.0f);
		Shader myShader_;
		Shader lightShader_;
		Cube lightCube_;
		Model myModel;
		Material myMaterial_;
		Light myLight_;
		float dt2 = 0;

	};

	void HelloSquare::Begin()
	{

		myShader_ = Shader("data/shaders/hello_lighting/LightMap.vert", "data/shaders/hello_lighting/02LightPoint.frag");
		myShader_.Use();
		myModel = Model("data/model/backpack/backpack.obj");
		myMaterial_.SetGenericMaterial();
		myShader_.SetFloat("material.shininess", 100.0f);
		/*myShader_.SetVec3("light.direction", glm::vec3( - 0.2f, -1.0f, -0.3f));*/

		lightShader_ = Shader("data/shaders/hello_lighting/LightSource.vert", "data/shaders/hello_lighting/LightSource.frag");
		lightCube_.CreateCube();

		myLight_.SetGenericLight();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void HelloSquare::Update(float dt)
	{
		myShader_.Use();
		myShader_.SetVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		myShader_.SetVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		myShader_.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		myShader_.SetVec3("light.position", camPos_);
		myShader_.SetVec3("light.direction", camFront_);
		myShader_.SetFloat("light.constant", 1.0f);
		myShader_.SetFloat("light.linear", 0.09f);
		myShader_.SetFloat("light.quadratic", 0.032f);

		myShader_.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		myShader_.SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		dt2 += dt;

		if (dt2 > 6.3f)
		{
			dt2 -= 6.3f;
		}

		lightPos_ = glm::vec3(2.0f * cos(dt2+1.5f), 0.75f, 2.0f * sin(dt2+1.5f));


		myShader_.SetVec3("viewPos", camPos_);
		myShader_.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		myLight_.position = lightPos_;
		myShader_.SetLight(myLight_);

		myShader_.SetProjViewMat(camProj_, camView_);
		myShader_.SetMat4("model", model_);


		myModel.Draw(myShader_);


		lightCube_.Use();
		lightShader_.Use();
		lightModel_ = glm::mat4(1.0f);
		lightModel_ = glm::translate(lightModel_, lightPos_);
		lightModel_ = glm::scale(lightModel_, glm::vec3(0.2f));


		lightShader_.SetProjViewMat(camProj_, camView_);
		lightShader_.SetMat4("model", lightModel_);
		lightCube_.Use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightCube_.Unbind();
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

