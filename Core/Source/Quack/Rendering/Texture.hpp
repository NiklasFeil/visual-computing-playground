#pragma once

#include <iostream>
#include <filesystem>
#include <GLAD/glad.h>

namespace Core {

    struct Texture {
        GLuint id;
        std::string type; // TODO: Use enum instead of string
        std::string path;

        void bind();
        void unbind();

        static GLuint from_file(const char* path, const std::string &directory, bool gamma);
    };
}