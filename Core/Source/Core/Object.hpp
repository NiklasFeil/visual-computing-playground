#pragma once
#include <memory>
#include <vector>
#include "Texture.hpp"
#include "Transform.hpp"
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
            Object(const ObjectRenderSpecs& render_specs, const Transform& transform, const TextureConfig& texture);
            Object(const ObjectRenderSpecs& render_specs, const glm::mat4& model, const TextureConfig& texture);
            ~Object();
            Object(const Object& other);
            Object(const Object&& other);
            Object& operator=(const Object& other) = delete;
            Object& operator=(const Object&& other) = delete;

            Transform& get_transform();

            void render();

        private:
            ObjectRenderSpecs m_render_specs; 
            Texture m_texture;
            Transform m_transform;

            unsigned int m_vao;
            unsigned int m_vbo;
            unsigned int m_ebo;
    };
}