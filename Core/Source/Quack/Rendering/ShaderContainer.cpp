#include "Rendering/ShaderContainer.hpp"

namespace Core {

    void ShaderContainer::add_shader(std::string name, Shader shader) {
        auto [it, inserted] = m_shader_mapping.try_emplace(std::move(name), std::move(shader));

        if (!inserted) {
            std::cerr << "Adding shader failed because of duplicate naming: " << name << "\nPlease remove old shader before adding new one." << std::endl;
            assert(false);
        }
    }

    
    void ShaderContainer::remove_shader(std::string name) {
        if (m_shader_mapping.contains(name)) {
            m_shader_mapping.erase(name);
        }
        else {
            std::cerr << "Removing shader failed because it does not exist: " << name << std::endl;
            assert(false);
        }
    }

    Shader& ShaderContainer::get_shader(std::string name) {
        if (m_shader_mapping.contains(name)) {
            return m_shader_mapping.at(name);
        }
        else {
            std::cerr << "Requested shader does not exist: " << name << std::endl;
            assert(false);
        }
    }

    void ShaderContainer::clear() {
        m_shader_mapping.clear();
    }
}