#pragma once

struct Material
{
	unsigned int texture0;
	unsigned int texture1;
	std::string textureName;
	std::string specularTexName;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	Material() = default;
	Material(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, float Shininess)
	{
		ambient = Ambient;
		diffuse = Diffuse;
		specular = Specular;
		shininess = Shininess;
	}
	Material(std::string TextureName, std::string SpecularTextureName, glm::vec3 Specular, float Shininess)
	{
		specularTexName = SpecularTextureName;
		textureName = TextureName;
		specular = Specular;
		shininess = Shininess;
	}
	void SetGenericMaterial();
};

struct Light
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light() = default;
	Light(glm::vec3 Position, glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular)
	{
		position = Position;
		ambient = Ambient;
		diffuse = Diffuse;
		specular = Specular;
	}
	void SetGenericLight();
};