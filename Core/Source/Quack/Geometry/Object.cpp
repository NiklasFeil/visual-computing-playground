#include "Geometry/Object.hpp"
#include <iostream>

namespace Core {
    Object::Object(const Model& model, const Transform& transform) 
    : m_model(model), m_transform(transform) {}

    Object::Object(const Model& model, const glm::mat4& model_matrix)
    : m_model(model), m_transform(Transform::FromModelMatrix(model_matrix)) {}

    void Object::render(Shader& shader) {
        shader.use();
        m_model.render(shader);
    }

    Object::~Object() {
        std::cout << "Object destroyed" << std::endl;
    }

    Transform& Object::getTransform() {
        return m_transform;
    }

}