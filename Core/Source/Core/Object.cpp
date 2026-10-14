#include "Object.hpp"
#include <GLAD/glad.h>
#include <utility>
#include <iostream>
#include <numeric>

namespace Core {
    Object::Object(const Model& model, const Transform& transform) 
    : m_model(model), m_transform(transform) {}

    Object::Object(const Model& model, const glm::mat4& model_matrix)
    : m_model(model), m_transform(Transform::from_model_matrix(model_matrix)) {}

    void Object::render(Shader& shader) {
        glEnable(GL_DEPTH_TEST);

        m_model.render(shader);
        
        glDisable(GL_DEPTH_TEST);
    }

    Object::~Object() {
        std::cout << "Object destroyed" << std::endl;
    }

    Transform& Object::get_transform() {
        return m_transform;
    }

}