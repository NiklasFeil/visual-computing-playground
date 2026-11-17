#pragma once 

#include <string>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GLAD/glad.h>
#include <filesystem>
#include "Camera/Camera.hpp"
#include "ExceptionHandler/ExceptionHandler.hpp"

namespace Core {

    class Shader {
        public:
            Shader(const std::filesystem::path vertex_shader_path, const std::filesystem::path fragment_shader_path);
            ~Shader();
            /*
            Shader(const Shader& other) = default;
            Shader(const Shader&& other) = default;
            Shader& operator= (const Shader& other) = default;
            Shader& operator= (const Shader&& other) = default; 
            */

            void use();
            void compile();

            template <typename T>
            void setUniform(std::string_view name, T value) {
                const GLint location = [&]() {
                    if(m_locations.contains(name.data()))
                        return m_locations.at(name.data());
                    
                    GLint location = glGetUniformLocation(m_id, name.data());
                    if (location != -1) {
                        m_locations.emplace(name.data(), location);
                    }
                    return location;
                }();
                
                if (location == -1) {
                    //Logger::Log(std::format("Uniform \"{}\" was not found", name), LogLevel::Warning);
                    return;
                }

                use();
                setUniformImpl(location, value);
            }

            void reload();

            unsigned int getId() {
                return m_id;
            }

            GLuint constructShaderProgram(std::string vertex_shader_code, std::string fragment_shader_code);

            static std::pair<std::string, std::string> ShaderCodeFromFile(const std::filesystem::path vertex_shader_path, const std::filesystem::path fragment_shader_path);

        private:
            unsigned int m_id;
            std::string m_vertex_shader_code;
            std::string m_fragment_shader_code;
            std::filesystem::path m_vertex_shader_path;
            std::filesystem::path m_fragment_shader_path;

            std::unordered_map<std::string, GLint> m_locations;

            bool m_use_fallback;

            // uniforms per type
            #include "Rendering/setUniformImpl.inl"
    };
}