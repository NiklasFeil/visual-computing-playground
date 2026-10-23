#include "Shader.hpp"
#include "ShaderContainer.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace Core {
    Shader::Shader(const std::filesystem::path vertex_shader_path, const std::filesystem::path fragment_shader_path) {

        if (!std::filesystem::exists(vertex_shader_path)) {
            std::cerr << "Given vertex shader path " << vertex_shader_path << " does not exist." << std::endl;
        }

        if (!std::filesystem::exists(fragment_shader_path)) {
            std::cerr << "Given fragment shader path " << fragment_shader_path << " does not exist." << std::endl;
        }

        std::ifstream vshader_file;
        std::ifstream fshader_file;
        vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // Vertex Shader
            // open files
            vshader_file.open(vertex_shader_path);
            std::stringstream vshader_stream;
            // read file's buffer contents into streams
            vshader_stream << vshader_file.rdbuf();
            // close file handler
            vshader_file.close();
            // convert stream into string
            m_vertex_shader_code = vshader_stream.str();
        }
        catch(std::ifstream::failure& e) {
            std::cout << "ERROR::vertex_shader::FILE_NOT_SUCCESSFULLY_READ | Error code: " << e.code() << std::endl;
        }

        try {
            // Fragment Shader
            // open files
            fshader_file.open(fragment_shader_path);
            std::stringstream fshader_stream;
            // read file's buffer contents into streams
            fshader_stream << fshader_file.rdbuf();
            // close file handler
            fshader_file.close();
            // convert stream into string
            m_fragment_shader_code = fshader_stream.str();
        }
        catch(std::ifstream::failure& e) {
            std::cout << "ERROR::fragment_shader::FILE_NOT_SUCCESSFULLY_READ | Error code: " << e.code() << std::endl;
        }
    }

    Shader::~Shader() {
        glDeleteShader(m_id);
    }

    void Shader::use() {
        glUseProgram(m_id);
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

    void Shader::set_camera_uniforms(Camera& camera) {
        set_uniform("uView", camera.get_view_matrix());
        set_uniform("uProjection", camera.get_projection_matrix());
        set_uniform("uViewPos", camera.get_position());
    }
}