#include "Scene/Entity.hpp"

namespace Core {
    Entity::Entity(const Model& model, const Transform& transform) 
    : m_model(model), m_transform(transform) {}

    Entity::Entity(const Model& model, const glm::mat4& model_matrix)
    : m_model(model), m_transform(Transform::FromModelMatrix(model_matrix)) {}

    void Entity::render(const Camera& camera, Shader& shader) {
        shader.use();

        shader.setUniform("uModel", m_transform.getModelMatrix());
        shader.setUniform("uView", camera.getViewMatrix());
        shader.setUniform("uProjection", camera.getProjectionMatrix());
        shader.setUniform("uViewPos", camera.getPosition());
    
        m_model.render(camera, shader);
    }

    Transform& Entity::getTransform() {
        return m_transform;
    }

}