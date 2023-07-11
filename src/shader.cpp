#include "shader.h"
#include <sstream>
#include "stb_image.h"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	GLuint vertex = glewInit();
	GLuint fragment = 0;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	ID = glCreateProgram();

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::LoadTexture(std::string texName)
{
	int width, height, nrChannels;
	std::ostringstream path;
	path << "data/textures/" << texName << ".jpg";
	unsigned char* data = stbi_load(path.str().c_str(), &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

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

	glUseProgram(ID);
	std::ostringstream textureUniformName;
	textureUniformName << "texture" << textureId_;
	glUniform1i(glGetUniformLocation(ID, textureUniformName.str().c_str()), textureId_); // set it manually
	textureId_ += 1;

	glActiveTexture(GL_TEXTURE0 + textureId_);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::LoadTexture(std::string texName, std::string uniformName)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	int width, height, nrChannels;
	std::ostringstream path;
	path << "data/textures/" << texName << ".jpg";
	unsigned char* data = stbi_load(path.str().c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), textureId_);
		glUseProgram(ID);
		if (textureId_ == 1)
		{
			glActiveTexture(GL_TEXTURE1);
		}
		else
			glActiveTexture(GL_TEXTURE0);
		textureId_ += 1;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);
	}
}

unsigned int Shader::LoadTextureRet(std::string texName)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	int width, height, nrChannels;
	std::ostringstream path;
	path << "data/textures/" << texName << ".jpg";
	unsigned char* data = stbi_load(path.str().c_str(), &width, &height, &nrChannels, 0);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path.str() << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetProjViewMat(glm::mat4 projection, glm::mat4 view)
{
	SetMat4("projection", projection);
	SetMat4("view", view);
}

void Shader::SetMaterial(const Material material) const
{
	SetVec3("material.ambient", glm::vec3(0.9f) * material.ambient);
	SetVec3("material.diffuse", glm::vec3(1.0f) * material.diffuse);
	SetVec3("material.specular", glm::vec3(1.0f) * material.specular);
	SetFloat("material.shininess", material.shininess);
}

void Shader::SetMaterialWithTexture(Material material)
{
	material.texture0 = LoadTextureRet(material.textureName);
	//SetInt("material.diffuse", LoadTextureRet(material.textureName));
	/*LoadTexture(material.textureName, "material.diffuse");*/
	//SetInt("material.specular", LoadTextureRet(material.specularTexName));
	material.texture1 = LoadTextureRet(material.specularTexName);
	/*LoadTexture(material.specularTexName, "material.specular");*/

	SetFloat("material.shininess", material.shininess);
}


void Shader::SetLight(const Light light) const
{
	SetVec3("light.position", light.position);
	SetVec3("light.ambient", light.ambient);
	SetVec3("light.diffuse", light.diffuse);
	SetVec3("light.specular", light.specular);
}
