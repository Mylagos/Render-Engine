#pragma once
#include "mesh.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model 
{
    public:
        std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh>    meshes;
        std::string directory;
        bool gammaCorrection;
        Model() = default;
        Model(const std::string &path, bool gamma = false) : gammaCorrection(gamma)
        {
            LoadModel(path);
        }
        void Draw(Shader &shader);	
    private:
        // model data
        void LoadModel(std::string const &path);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};