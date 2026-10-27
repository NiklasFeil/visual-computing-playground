#pragma once 

#include "Rendering/Shader.hpp"
#include <iostream>

namespace Core {
 
    class ShaderContainer {
        public:
            ShaderContainer() {}
            ~ShaderContainer() { m_shader_mapping.clear(); }

            void add_shader(std::string name, Shader shader);
            
            void remove_shader(std::string name);

            Shader& get_shader(std::string name);

            void clear();
            

        private:
            std::unordered_map<std::string, Shader> m_shader_mapping{};
    };

}