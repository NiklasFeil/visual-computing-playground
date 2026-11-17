#pragma once

#include "Scene/Entity.hpp"

namespace Core {

    
    class SceneGraph {
        
        public:
        
            using EntityMap = std::unordered_map<std::string, std::unique_ptr<Entity>>;
            using EntityIterator = EntityMap::const_iterator;

            SceneGraph();

            ~SceneGraph() { m_entities.clear(); }

            template <typename eT, typename ... Args>
            void add_entity(std::string name, Args&& ... args) {
                auto [it, inserted] = m_entities.try_emplace(name, std::move(std::make_unique<eT>(std::forward<Args>(args)...)));

                if(!inserted) {
                    Logger::Log(std::format("Adding entity {} to the scene graph failed due to duplicate naming.", name), LogLevel::Warning);
                }
            }

            void remove_entity(std::string name);

            Entity& get_entity(std::string name);

            const std::ranges::subrange<EntityIterator> get_entity_range() const;

            void clear();

        private:
            EntityMap m_entities;

    };

}