#pragma once
#include <string>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Core {
    class Model {
        public:
            Model(std::string path);

            void render(Shader& shader);

            std::vector<Texture> textures_loaded;
            
        private:
            std::vector<Mesh> m_meshes;
            std::string m_directory;

            void loadModel(std::string path);
            void processNode(aiNode *node, const aiScene *scene);
            Mesh processMesh(aiMesh *mesh, const aiScene *scene);
            std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    };
}