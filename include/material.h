#pragma once

struct Material
{
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