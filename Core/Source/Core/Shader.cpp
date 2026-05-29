#include "Shader.hpp"
#include "GLAD/glad.h"
#include <iostream>

namespace Core {
    Shader::Shader(ShaderConfig&& config)
    : m_vertex_shader_code(config.vertex_shader_code), m_fragment_shader_code(config.fragment_shader_code) {}

    Shader::~Shader() {
        glDeleteShader(m_id);
    }

    void Shader::compile() {
    
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        auto vs_code = m_vertex_shader_code.c_str();
        
        glShaderSource(vertexShader, 1, &vs_code, NULL);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            // TODO: Throw proper error
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        auto fs_code = m_fragment_shader_code.c_str();
        glShaderSource(fragmentShader, 1, &fs_code, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            // TODO: Throw proper error
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        m_id = glCreateProgram();

        glAttachShader(m_id, vertexShader);
        glAttachShader(m_id, fragmentShader);
        glLinkProgram(m_id);

        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }
}