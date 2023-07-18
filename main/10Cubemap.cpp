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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace gpr5300
{
	unsigned int loadCubemap(std::vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}

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
		Shader skyBoxShader_;
		Cube litCube_;
		Cube lightCube_;
		Material myMaterial_;
		Light myLight_;
		float dt2 = 0;
		unsigned int cubemapTexture_;
		unsigned int skyBoxVAO_;

	};

	void HelloSquare::Begin()
	{
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		std::vector<std::string> faces
		{
				"data/textures/skybox/right.jpg",
				"data/textures/skybox/left.jpg",
				"data/textures/skybox/top.jpg",
				"data/textures/skybox/bottom.jpg",
				"data/textures/skybox/front.jpg",
				"data/textures/skybox/back.jpg"
		};
		cubemapTexture_ = loadCubemap(faces);

		skyBoxShader_ = Shader("data/shaders/adv_opengl/cubemap.vert", "data/shaders/adv_opengl/cubemap.frag");
		skyBoxShader_.Use();
		skyBoxShader_.SetInt("skybox", 0);

		unsigned int skyboxVBO;
		glGenVertexArrays(1, &skyBoxVAO_);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyBoxVAO_);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


		myShader_ = Shader("data/shaders/adv_opengl/reflection.vert", "data/shaders/adv_opengl/refraction.frag");
		myShader_.Use();
		litCube_.CreateNormalCube();
		myShader_.SetInt("skybox", 0);
		
		//myMaterial_.SetGenericMaterial();
		//myMaterial_.textureName = "zizou_uzumaki";
		//myMaterial_.specularTexName = "box_specular";
		//myMaterial_.texture0 = Shader::LoadTextureRet(myMaterial_.textureName);
		//myMaterial_.texture1 = Shader::LoadTextureRet(myMaterial_.specularTexName);
		//myShader_.SetInt("material.diffuse", 0);
		//myShader_.SetInt("material.specular", 1);
		//myShader_.SetFloat("material.shininess", myMaterial_.shininess);
		///*myShader_.SetVec3("light.direction", glm::vec3( - 0.2f, -1.0f, -0.3f));*/
		//myShader_.SetVec3("light.position", camPos_);
		//myShader_.SetVec3("light.direction", camFront_);
		//myShader_.SetFloat("light.constant", 1.0f);
		//myShader_.SetFloat("light.linear", 0.09f);
		//myShader_.SetFloat("light.quadratic", 0.032f);
		//myShader_.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));

		/*lightShader_ = Shader("data/shaders/hello_lighting/LightSource.frag", "data/shaders/hello_lighting/LightSource.frag");
		lightCube_.CreateCube();

		myLight_.SetGenericLight();*/

		model2_ = glm::translate(model2_, glm::vec3(1.0, 0.0, -2.0));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void HelloSquare::Update(float dt)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);

		myShader_.Use();
		/*myShader_.SetVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		myShader_.SetVec3("light.diffuse", glm::vec3(1.0f));
		myShader_.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		myShader_.SetVec3("light.position", camPos_);
		myShader_.SetVec3("light.direction", camFront_);
		myShader_.SetFloat("light.constant", 1.0f);
		myShader_.SetFloat("light.linear", 0.09f);
		myShader_.SetFloat("light.quadratic", 0.032f);

		myShader_.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		myShader_.SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));*/

		dt2 += dt;

		if (dt2 > 6.3f)
		{
			dt2 -= 6.3f;
		}

		lightPos_ = glm::vec3(1.0f * cos(dt2+1.5f), 0.75f, 1.0f * sin(dt2+1.5f));


		litCube_.Use();

		/*myShader_.SetVec3("viewPos", camPos_);
		myShader_.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		myLight_.position = lightPos_;*/
		//myShader_.SetLight(myLight_);

		myShader_.SetProjViewMat(camProj_, camView_);
		myShader_.SetMat4("model", model1_);
		myShader_.SetVec3("cameraPos", camPos_);


		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myMaterial_.texture0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, myMaterial_.texture1);*/

		litCube_.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		myShader_.SetMat4("model", model2_);

		glFrontFace(GL_CCW);

		litCube_.Use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		litCube_.Unbind();

		glFrontFace(GL_CW);

		/*lightCube_.Use();
		lightShader_.Use();
		lightModel_ = glm::mat4(1.0f);
		lightModel_ = glm::translate(lightModel_, lightPos_);
		lightModel_ = glm::scale(lightModel_, glm::vec3(0.2f));*/


		/*lightShader_.SetProjViewMat(camProj_, camView_);
		lightShader_.SetMat4("model", lightModel_);
		lightCube_.Use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightCube_.Unbind();*/

		glDepthFunc(GL_LEQUAL);
		camView_ = glm::mat4(glm::mat3(camView_));
		skyBoxShader_.Use();
		skyBoxShader_.SetProjViewMat(camProj_, camView_);
		glBindVertexArray(skyBoxVAO_);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
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

