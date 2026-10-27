#pragma once

#include "Geometry/Transform.hpp"
#include "Rendering/Shader.hpp"
#include "glm/vec3.hpp"

namespace Core {

    struct PrimitiveVertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    class Primitive {
        public:
            Primitive(const Transform& transform, const glm::vec3& color)
                : m_transform(transform), m_color(color) {}

            virtual ~Primitive() = default;

            Transform& get_transform() {
                return m_transform;
            }

            const glm::vec3& get_color() const {
                return m_color;
            }

            virtual void generate_vertices() {}
            void construct_model();

            void render(Shader& shader);

        protected:
            Transform m_transform;
            glm::vec3 m_color;

            std::vector<PrimitiveVertex> m_vertices;
            std::vector<unsigned int> m_indices;

            // render data
            unsigned int m_vao, m_vbo, m_ebo;

    };

    class Cuboid : public Primitive {
        public:
            Cuboid(const Transform& transform, const glm::vec3& color)
                : Primitive(transform, color) {
                    generate_vertices();
                    construct_model();
                }

            void generate_vertices() override;
    };

    class Sphere : public Primitive {
        public:
            Sphere(const Transform& transform, const glm::vec3& color)
                : Primitive(transform, color) {
                    generate_vertices();
                    construct_model();
                }

            void generate_vertices() override;
    };

    class Cylinder : public Primitive {
        public:
            Cylinder(const Transform& transform, const glm::vec3& color)
                : Primitive(transform, color) {
                    generate_vertices();
                    construct_model();
                }

            void generate_vertices() override;
    };

    class Cone : public Primitive {
        public:
            Cone(const Transform& transform, const glm::vec3& color)
                : Primitive(transform, color) {
                    generate_vertices();
                    construct_model();
                }

            void generate_vertices() override;
    };
}
