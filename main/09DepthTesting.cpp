#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/matrix.hpp>

#include "engine.h"
#include "scene.h"
#include "cube.h"

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
		glm::mat4 model1_ = glm::mat4(1.0f);
		glm::mat4 model2_ = glm::mat4(1.0f);
		glm::mat4 model3_ = glm::mat4(1.0f);
		glm::mat4 lightModel_ = glm::mat4(1.0f);
		glm::vec3 lightPos_ = glm::vec3(1.0f, 1.0f, 0.0f);
		Shader myShader_;
		Shader lightShader_;
		Cube litCube_;
		Cube lightCube_;
		Material myMaterial_;
		Light myLight_;
		float dt2 = 0;

	};

	void HelloSquare::Begin()
	{

		myShader_ = Shader("data/shaders/hello_lighting/LightMap.vert", "data/shaders/hello_lighting/04LightSpotSoft.frag");
		myShader_.Use();
		litCube_.CreateNormalMapCube();
		
		myMaterial_.SetGenericMaterial();
		myMaterial_.textureName = "zizou_uzumaki";
		myMaterial_.specularTexName = "box_specular";
		myMaterial_.texture0 = Shader::LoadTextureRet(myMaterial_.textureName);
		myMaterial_.texture1 = Shader::LoadTextureRet(myMaterial_.specularTexName);
		myShader_.SetInt("material.diffuse", 0);
		myShader_.SetInt("material.specular", 1);
		myShader_.SetFloat("material.shininess", myMaterial_.shininess);
		/*myShader_.SetVec3("light.direction", glm::vec3( - 0.2f, -1.0f, -0.3f));*/
		myShader_.SetVec3("light.position", camPos_);
		myShader_.SetVec3("light.direction", camFront_);
		myShader_.SetFloat("light.constant", 1.0f);
		myShader_.SetFloat("light.linear", 0.09f);
		myShader_.SetFloat("light.quadratic", 0.032f);
		myShader_.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));

		lightShader_ = Shader("data/shaders/hello_lighting/LightSource.vert", "data/shaders/hello_lighting/LightSource.frag");
		lightCube_.CreateCube();

		myLight_.SetGenericLight();

		model2_ = glm::translate(model2_, glm::vec3(1.0, 0.0, -2.0));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void HelloSquare::Update(float dt)
	{
		myShader_.Use();
		myShader_.SetVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		myShader_.SetVec3("light.diffuse", glm::vec3(1.0f));
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
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);

		dt2 += dt;

		if (dt2 > 6.3f)
		{
			dt2 -= 6.3f;
		}

		lightPos_ = glm::vec3(1.0f * cos(dt2+1.5f), 0.75f, 1.0f * sin(dt2+1.5f));


		litCube_.Use();

		myShader_.SetVec3("viewPos", camPos_);
		myShader_.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		myLight_.position = lightPos_;
		//myShader_.SetLight(myLight_);

		myShader_.SetProjViewMat(camProj_, camView_);
		myShader_.SetMat4("model", model1_);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myMaterial_.texture0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, myMaterial_.texture1);


		litCube_.Use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		myShader_.SetMat4("model", model2_);

		litCube_.Use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		litCube_.Unbind();


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

