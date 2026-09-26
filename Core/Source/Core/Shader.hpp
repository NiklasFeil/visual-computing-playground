#pragma once 

#include <string>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GLAD/glad.h>
#include <filesystem>

namespace Core {
    struct ShaderConfig {
        std::filesystem::path vertex_shader_path;
        std::filesystem::path fragment_shader_path;
    };

    class Shader {
        public:
            Shader(const ShaderConfig& config);
            ~Shader();
            Shader(const Shader& other) = delete;
            Shader(const Shader&& other) = delete;
            Shader& operator= (const Shader& other) = delete;
            Shader& operator= (const Shader&& other) = delete; 

            void use();
            void compile();

            template <typename T>
            void set_uniform(std::string_view name, T value) {
                const int location = glGetUniformLocation(m_id, name.data());
                set_uniform_impl(location, value);
            }

            unsigned int get_id() {
                return m_id;
            }
        private:
            unsigned int m_id;
            std::string m_vertex_shader_code;
            std::string m_fragment_shader_code;

            // uniforms per type
            inline void set_uniform_impl(const int location, const float v) const {
                glUniform1f(location, v);
            }
            inline void set_uniform_impl(const int location, const glm::vec2 v) const {
                glUniform2f(location, v.x, v.y);
            }
            inline void set_uniform_impl(const int location, const glm::vec3 v) const {
                glUniform3f(location, v.x, v.y, v.z);
            }
            inline void set_uniform_impl(const int location, const glm::vec4 v) const {
                glUniform4f(location, v.x, v.y, v.z, v.w);
            }
            
            inline void set_uniform_impl(const int location, const int v) const {
                glUniform1i(location, v);
            }
            inline void set_uniform_impl(const int location, const glm::ivec2 v) const {
                glUniform2i(location, v.x, v.y);
            }
            inline void set_uniform_impl(const int location, const glm::ivec3 v) const {
                glUniform3i(location, v.x, v.y, v.z);
            }
            inline void set_uniform_impl(const int location, const glm::ivec4 v) const {
                glUniform4i(location, v.x, v.y, v.z, v.w);
            }

            inline void set_uniform_impl(const int location, const unsigned int v) const {
                glUniform1ui(location, v);
            }
            inline void set_uniform_impl(const int location, const glm::uvec2 v) const {
                glUniform2ui(location, v.x, v.y);
            }
            inline void set_uniform_impl(const int location, const glm::uvec3 v) const {
                glUniform3ui(location, v.x, v.y, v.z);
            }
            inline void set_uniform_impl(const int location, const glm::uvec4 v) const {
                glUniform4ui(location, v.x, v.y, v.z, v.w);
            }

            inline void set_uniform_impl(const int location, const glm::mat4 v) const {
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
            }
    };
}