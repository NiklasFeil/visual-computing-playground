#include "Cuboid.hpp"

namespace Core {

    Model get_cuboid_model(glm::vec3 color, bool inverted) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::tie(vertices, indices) = Mesh::Cuboid(inverted);
        MaterialSimple mat(0.05f * color, color, 0.2f * color, 1.0f);
        Mesh mesh(vertices, indices, mat);
        Model model(mesh);
        return model;
    }

    Transform get_cuboid_transform(glm::vec3 pos, glm::vec3 dim) {
        Transform t = Transform::FromPosition(pos);
        t.scale(dim);
        return t;
    }

    Cuboid::Cuboid(glm::vec3 pos, glm::vec3 dim, glm::vec3 color, bool inverted = false)
    : Entity(get_cuboid_model(color, inverted), get_cuboid_transform(pos, dim)) {}
      
}