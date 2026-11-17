#pragma once
#include "Geometry/Transform.hpp"
#include "Geometry/Model.hpp"
#include "glm/glm.hpp"

namespace Core {

    class Entity {
        public:
            Entity(const Model& model, const Transform& transform);
            Entity(const Model& model, const glm::mat4& model_matrix);
            
            virtual ~Entity() = default;
            Entity(const Entity& other);
            Entity(const Entity&& other);
            Entity& operator=(const Entity& other) = delete;
            Entity& operator=(const Entity&& other) = delete;

            Transform& getTransform();

            void render(const Camera& camera, Shader& shader);

        private:
            Model m_model;
            Transform m_transform;

            unsigned int m_vao;
            unsigned int m_vbo;
            unsigned int m_ebo;
    };
}