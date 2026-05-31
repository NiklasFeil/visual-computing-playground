#pragma once
#include <memory>
#include <vector>
//#include <glm/glm.hpp>

namespace Core {
    struct ObjectRenderSpecs {
        std::vector<float> vertices;
        std::vector<int> indices;
        int drawing_method;
        std::vector<unsigned int> elements_per_vertex_attribute;  
    };

    struct Transform {
//        glm::vec3 position;
    };

    class Object {
        public:
            Object(ObjectRenderSpecs& render_specs, Transform& transform);
            ~Object();
            Object(const Object& other);
            Object(const Object&& other);
            Object& operator=(const Object& other) = delete;
            Object& operator=(const Object&& other) = delete;

            void render();

        private:
            std::unique_ptr<ObjectRenderSpecs> m_render_specs; 
            std::unique_ptr<Transform> m_transform; 

            unsigned int m_vao;
            unsigned int m_vbo;
            unsigned int m_ebo;
    };
}