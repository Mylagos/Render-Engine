#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SDL.h>
#include "material.h"


class Shader
{
private:
    int textureId_ = 0;
public:
    // the program ID
    GLuint ID;

    // constructor reads and builds the shader
    Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void Use() const;
    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, glm::vec3 value) const;
    void LoadTexture(std::string texName);
    void LoadTexture(std::string texName, std::string uniformName);
    static unsigned int LoadTextureRet(std::string texName);
    void SetMat4(const std::string& name, glm::mat4 value) const;
    void SetMat3(const std::string& name, glm::mat3 value) const;
    void SetProjViewMat(glm::mat4 projection, glm::mat4 view);
    void SetMaterial(const Material material) const;
    void SetMaterialWithTexture(Material material);
    void SetLight(const Light light) const;
};
