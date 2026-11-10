#include "Rendering/Shader.hpp"
#include "Rendering/ShaderContainer.hpp"
#include "ExceptionHandler/ExceptionHandler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Core {
    Shader::Shader(const std::filesystem::path vertex_shader_path, const std::filesystem::path fragment_shader_path)
    : m_vertex_shader_path(vertex_shader_path), m_fragment_shader_path(fragment_shader_path), m_use_fallback(false) {
        std::tie(m_vertex_shader_code, m_fragment_shader_code) = ShaderCodeFromFile(m_vertex_shader_path, m_fragment_shader_path);
    }

    Shader::~Shader() {
        glDeleteShader(m_id);
    }

    void Shader::use() {
        glUseProgram(m_id);
    }

    void Shader::compile() {
        m_id = constructShaderProgram(m_vertex_shader_code, m_fragment_shader_code);
    }

    GLuint Shader::constructShaderProgram(std::string vertex_shader_code, std::string fragment_shader_code) {
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        auto vs_code = vertex_shader_code.c_str();
        
        glShaderSource(vertexShader, 1, &vs_code, NULL);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            // TODO: Throw proper error
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            //std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            ExceptionHandler::Throw(std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"), "Failed compiling vertex shader");
        }

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        auto fs_code = fragment_shader_code.c_str();
        glShaderSource(fragmentShader, 1, &fs_code, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            // TODO: Throw proper error
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            // std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            ExceptionHandler::Throw(std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"), "Failed compiling fragment shader");
        }

        GLuint id = glCreateProgram();

        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            //std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            ExceptionHandler::Throw(std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n"), "Failed linking shader program");
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return id;
    }

    void Shader::setCameraUniforms(Camera& camera) {
        setUniform("uView", camera.getViewMatrix());
        setUniform("uProjection", camera.getProjectionMatrix());
        setUniform("uViewPos", camera.getPosition());
    }

    std::pair<std::string, std::string> Shader::ShaderCodeFromFile(const std::filesystem::path vertex_shader_path, const std::filesystem::path fragment_shader_path) {

        ExceptionHandler::Assert(std::filesystem::exists(vertex_shader_path), "Path to vertex shader does not exist.");
        ExceptionHandler::Assert(std::filesystem::exists(fragment_shader_path), "Path to fragment shader does not exist.");

        std::ifstream vshader_file;
        std::ifstream fshader_file;
        vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::string vertex_shader_code, fragment_shader_code;

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
            vertex_shader_code = vshader_stream.str();
        }
        catch(std::ifstream::failure& e) {
            //std::cout << "ERROR::vertex_shader::FILE_NOT_SUCCESSFULLY_READ | Error code: " << e.code() << std::endl;
            //ExceptionHandler::Throw(e, std::format("Could not read vertex shader file successfully | Error code: {}", e.code()));
            ExceptionHandler::Throw(e, "Could not read vertex shader file successfully.");
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
            fragment_shader_code = fshader_stream.str();
        }
        catch(std::ifstream::failure& e) {
            //std::cout << "ERROR::fragment_shader::FILE_NOT_SUCCESSFULLY_READ | Error code: " << e.code() << std::endl;
            //ExceptionHandler::Throw(e, std::format("Could not read fragment shader file successfully | Error code: {}", e.code()));
            ExceptionHandler::Throw(e, "Could not read fragment shader file successfully.");
        }

        return std::make_pair(vertex_shader_code, fragment_shader_code);
    }

    void Shader::reload() {
        std::string vertex_shader_code, fragment_shader_code;
        GLuint new_program;

        try {
            std::tie(vertex_shader_code, fragment_shader_code) = ShaderCodeFromFile(m_vertex_shader_path, m_fragment_shader_path);
            new_program = constructShaderProgram(vertex_shader_code, fragment_shader_code);
        } catch(std::runtime_error e) {
            Logger::Log("Reloading shader did not work. Using fallback...", LogLevel::Warning);
            std::filesystem::path vertex_fallback_path = "Core/Shaders/fallback.vert";
            std::filesystem::path fragment_fallback_path = "Core/Shaders/fallback.frag";
            std::tie(vertex_shader_code, fragment_shader_code) = ShaderCodeFromFile(vertex_fallback_path, fragment_fallback_path);
            new_program = constructShaderProgram(vertex_shader_code, fragment_shader_code);
        }

        if (new_program) {
            glDeleteProgram(m_id);
            m_id = new_program;
        }
    }
}