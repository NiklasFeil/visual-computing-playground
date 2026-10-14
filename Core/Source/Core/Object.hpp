#pragma once
#include <memory>
#include <vector>
#include "Texture.hpp"
#include "Transform.hpp"
#include "Model.hpp"
#include "glm/glm.hpp"

namespace Core {
    struct ObjectRenderSpecs {
        std::vector<float> vertices;
        std::vector<int> indices;
        int drawing_method;
        std::vector<unsigned int> elements_per_vertex_attribute;  
    };

    class Object {
        public:
            Object(const Model& model, const Transform& transform);
            Object(const Model& model, const glm::mat4& model_matrix);
            
            ~Object();
            Object(const Object& other);
            Object(const Object&& other);
            Object& operator=(const Object& other) = delete;
            Object& operator=(const Object&& other) = delete;

            Transform& get_transform();

            void render(Shader& shader);

        private:
            Model m_model;
            Transform m_transform;

            unsigned int m_vao;
            unsigned int m_vbo;
            unsigned int m_ebo;
    };
}