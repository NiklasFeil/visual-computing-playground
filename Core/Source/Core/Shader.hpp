#pragma once 

#include <string>

namespace Core {
    struct ShaderConfig {
        std::string vertex_shader_code;
        std::string fragment_shader_code;  
    };

    class Shader {
        public:
            Shader(ShaderConfig&& config);
            ~Shader();
            Shader(const Shader& other) = delete;
            Shader(const Shader&& other) = delete;
            Shader& operator= (const Shader& other) = delete;
            Shader& operator= (const Shader&& other) = delete; 

            void compile();
            unsigned int get_id() {
                return m_id;
            }
        private:
            unsigned int m_id;
            std::string m_vertex_shader_code;
            std::string m_fragment_shader_code;
    };
}