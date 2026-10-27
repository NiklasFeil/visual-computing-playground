#include "Geometry/Model.hpp"
#include "assimp/Importer.hpp"
#include <iostream>

namespace Core {

    Model::Model(std::string path, bool flip_uvs) {
        loadModel(path, flip_uvs);
    }

    void Model::loadModel(std::string path, bool flip_uvs) {
        
        Assimp::Importer import;
        
        // TODO: Add options when loading to support more configurations.
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | (aiProcess_FlipUVs * flip_uvs));

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }

        m_directory = path.substr(0, path.find_last_of("/"));
        processNode(scene->mRootNode, scene);
        
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {
        
        
        // process all the node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(processMesh(mesh, scene));
        }

        // continue with the children
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
        
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        
        // Process Vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;

            // Position
            const auto v = mesh->mVertices[i];
            vertex.position = glm::vec3(v.x, v.y, v.z);

            // Normal
            const auto n = mesh->mNormals[i];
            vertex.normal = glm::vec3(n.x, n.y, n.z);

            // UV
            if (mesh->mTextureCoords[0]) {
                const auto uv = mesh->mTextureCoords[0][i];
                vertex.tex_coords = glm::vec2(uv.x, uv.y);

                const auto t = mesh->mTangents[i];
                vertex.tangent = glm::vec3(t.x, t.y, t.z);

                const auto bt = mesh->mBitangents[i];
                vertex.bitangent = glm::vec3(bt.x, bt.y, bt.z);
            }
            else {
                vertex.tex_coords = glm::vec2(0.0, 0.0);
            }

            vertices.push_back(vertex);
        }

        // Process Indices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Process Material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            std::vector<Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

            std::vector<Texture> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
        }

        return Mesh(std::move(vertices), std::move(indices), std::move(textures));
    }

    std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {

        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {

            aiString str;
            mat->GetTexture(type, i, &str);
            
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); ++j) {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                Texture tex;
                tex.id = Texture::from_file(str.C_Str(), m_directory, false);
                tex.type = typeName;
                tex.path = str.C_Str();
                textures.push_back(tex);
                textures_loaded.push_back(tex);
            }
        }

        return textures; // Copy Elision constructs vector directly in caller's memory. No need to std::move.
    }       

    void Model::render(Shader& shader) {
        for(Mesh& mesh: m_meshes)
            mesh.render(shader);
    }
}