#pragma once
#include "Geometry/Transform.hpp"
#include "Geometry/Model.hpp"
#include "glm/glm.hpp"

namespace Core {

    class Object {
        public:
            Object(const Model& model, const Transform& transform);
            Object(const Model& model, const glm::mat4& model_matrix);
            
            ~Object();
            Object(const Object& other);
            Object(const Object&& other);
            Object& operator=(const Object& other) = delete;
            Object& operator=(const Object&& other) = delete;

            Transform& getTransform();

            void render(Shader& shader);

        private:
            Model m_model;
            Transform m_transform;

            unsigned int m_vao;
            unsigned int m_vbo;
            unsigned int m_ebo;
    };
}