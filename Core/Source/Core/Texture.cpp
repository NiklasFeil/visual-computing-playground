#include "Texture.hpp"
#include <GLAD/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include <iostream>

namespace Core {

    Texture::Texture(const TextureConfig& tex_config) {

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(tex_config.path.c_str(), &width, &height, &nrChannels, 0);
        stbi_set_flip_vertically_on_load(false);
        
        if(data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture() {
        std::cout << "Texture destroyed" << std::endl;
        glDeleteTextures(1, &m_id);
    }

    void Texture::bind() {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}