#include "Scene/SceneGraph.hpp"

namespace Core {
    
    SceneGraph::SceneGraph() {}

    void SceneGraph::remove_entity(std::string name) {
        if (m_entities.contains(name)) {
            m_entities.erase(name);
        }
        else {
            std::cerr << "Removing shader failed because it does not exist: " << name << std::endl;
            assert(false);
        }
    }

    Entity& SceneGraph::get_entity(std::string name) {
        if (m_entities.contains(name)) {
            return *m_entities.at(name).get();
        }
        else {
            Logger::Log(std::format("Requested entity does not exist: {}", name), LogLevel::Error);
            assert(false);
        }
    }

    const std::ranges::subrange<SceneGraph::EntityIterator> SceneGraph::get_entity_range() const {
        return {m_entities.cbegin(), m_entities.cend()};
    }

    void SceneGraph::clear() {
        m_entities.clear();
    }

}