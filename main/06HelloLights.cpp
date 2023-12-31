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
		glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 lightModel_ = glm::mat4(1.0f);
		glm::vec3 lightPos_ = glm::vec3(1.0f, 1.0f, 0.0f);
		Shader myShader_;
		Shader lightShader_;
		Cube litCube_;
		Cube lightCube_;
		Light myLight_;
		float dt2 = 0;

	};

	void HelloSquare::Begin()
	{

		myShader_ = Shader("data/shaders/hello_lighting/LitObject.vert", "data/shaders/hello_lighting/LitObject.frag");

		lightShader_ = Shader("data/shaders/hello_lighting/LightSource.vert", "data/shaders/hello_lighting/LightSource.frag");
		myLight_.SetGenericLight();


		myShader_.Use();
		Material myMaterial;
		myMaterial.SetGenericMaterial();
		myShader_.SetMaterial(myMaterial);

		litCube_.CreateNormalCube();
		lightCube_.CreateCube();

		model_ = glm::scale(model_, glm::vec3(3.0f));

		myShader_.LoadTexture("zizou_cropped");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void HelloSquare::Update(float dt)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		dt2 += dt;

		if (dt2 > 6.3f)
		{
			dt2 -= 6.3f;
		}

		lightPos_ = glm::vec3(4.0f * cos(dt2), 0.75f, 4.0f * sin(dt2));

		myShader_.Use();
		litCube_.Use();
		myShader_.SetVec3("viewPos", camPos_);
		myShader_.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		myLight_.position = lightPos_;
		myShader_.SetLight(myLight_);

		myShader_.SetProjViewMat(camProj_, camView_);
		myShader_.SetMat4("model", model_);


		glDrawArrays(GL_TRIANGLES, 0, 36);
		litCube_.Unbind();

		lightShader_.Use();
		lightCube_.Use();

		

		lightModel_ = glm::mat4(1.0f);
		lightModel_ = glm::translate(lightModel_, lightPos_);
		lightModel_ = glm::scale(lightModel_, glm::vec3(0.2f));


		lightShader_.SetProjViewMat(camProj_, camView_);
		lightShader_.SetMat4("model", lightModel_);

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

