#include "material.h"

void Material::SetGenericMaterial()
{
	ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	specular = glm::vec3(0.5f, 0.5f, 0.5f);
	shininess = 32.0f;
}

void Light::SetGenericLight()
{
	ambient = glm::vec3(0.35f, 0.35f, 0.35f);
	diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);
}
