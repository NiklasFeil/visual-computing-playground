#pragma once

#include <filesystem>
#include <GLAD/glad.h>

namespace Core {
 
    struct TextureConfig {
        std::filesystem::path path;
    };

    class Texture {
        public:
            Texture(const TextureConfig& tex_config);
            ~Texture();
            /*Texture(const Texture& other) = delete;
            Texture(const Texture&& other) = default;
            Texture& operator= (const Texture& other) = delete;
            Texture& operator= (const Texture&& other) = default;*/

            void bind();
            void unbind();

        private:
            GLuint m_id;
    };
}