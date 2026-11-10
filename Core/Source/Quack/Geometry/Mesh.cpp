#include "Geometry/Mesh.hpp"
#include <cstddef>

namespace Core {

    // Loaded from file using code in Model
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_textures(std::move(textures)), m_use_color(false) {
        setup_mesh();
    }

        Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Color color)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_color(color), m_use_color(true) {
        setup_mesh();
    }

    void Mesh::setup_mesh() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);
    
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                    &m_indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
        // vertex tangent
        glEnableVertexAttribArray(3);	
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);	
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        /*
        Introduce later
        // ids
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

		// weights
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        */

        glBindVertexArray(0);
    }

    void Mesh::render(Shader& shader) {
        
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        if (!m_use_color) {
            for(unsigned int i = 0; i < m_textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                std::string number;
                std::string name = m_textures[i].type; // TODO: enum instead of string
                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);
                else if(name == "texture_normal")
                    number = std::to_string(normalNr++);
                else if(name == "texture_height")
                    number = std::to_string(heightNr++);

                shader.setUniform(("uMaterial." + name + number).c_str(), i);
                //shader.setUniform((name + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
            }
        }
        else {
            shader.setUniform("uObjectColor", m_color);
        }

        shader.setUniform("uUseColor", m_use_color);

        // draw mesh
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }   

    Mesh Mesh::Cuboid(std::variant<Texture, Color> tex_or_color, bool inverted = false) {

        std::vector<Vertex> vertices;
        vertices.reserve(24);
        std::vector<unsigned int> indices;
        indices.reserve(36);

        struct Face {
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec3 bitangent;
            glm::vec3 corners[4]; // ccw
            glm::vec2 corner_uvs[4]; // ccw
        };

        constexpr glm::vec2 corner_uvs[4] = {
            {0.0f, 0.0f},   // corner 0
            {1.0f, 0.0f},   // corner 1  (+U = tangent direction)
            {1.0f, 1.0f},   // corner 2
            {0.0f, 1.0f},   // corner 3  (+V = bitangent direction)
        };

        const float h = 0.5f;
        std::array<Face, 6> faces = [&]() -> std::array<Face, 6> {

            if (inverted) {
                return {{
                    // TBN left-handed !ERROR SOURCE! later maybe, depending on how normal mapping is implemented 
                    {{-1, 0, 0}, { 0, 0,-1}, { 0, 1, 0}, {{ h,-h, h}, { h,-h,-h}, { h, h,-h}, { h, h, h}}},  // +X
                    {{ 1, 0, 0}, { 0, 0, 1}, { 0, 1, 0}, {{-h,-h,-h}, {-h,-h, h}, {-h, h, h}, {-h, h,-h}}},  // -X
                    {{ 0,-1, 0}, { 1, 0, 0}, { 0, 0,-1}, {{-h, h, h}, { h, h, h}, { h, h,-h}, {-h, h,-h}}},  // +Y
                    {{ 0, 1, 0}, { 1, 0, 0}, { 0, 0, 1}, {{-h,-h,-h}, { h,-h,-h}, { h,-h, h}, {-h,-h, h}}},  // -Y
                    {{ 0, 0,-1}, { 1, 0, 0}, { 0, 1, 0}, {{-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}}},  // +Z
                    {{ 0, 0, 1}, {-1, 0, 0}, { 0, 1, 0}, {{ h,-h,-h}, {-h,-h,-h}, {-h, h,-h}, { h, h,-h}}},  // -Z
                }};
            }
            else {
                return {{
                    // TBN right-handed
                    {{ 1, 0, 0}, { 0, 0,-1}, { 0, 1, 0}, {{ h,-h, h}, { h,-h,-h}, { h, h,-h}, { h, h, h}}},  // +X
                    {{-1, 0, 0}, { 0, 0, 1}, { 0, 1, 0}, {{-h,-h,-h}, {-h,-h, h}, {-h, h, h}, {-h, h,-h}}},  // -X
                    {{ 0, 1, 0}, { 1, 0, 0}, { 0, 0,-1}, {{-h, h, h}, { h, h, h}, { h, h,-h}, {-h, h,-h}}},  // +Y
                    {{ 0,-1, 0}, { 1, 0, 0}, { 0, 0, 1}, {{-h,-h,-h}, { h,-h,-h}, { h,-h, h}, {-h,-h, h}}},  // -Y
                    {{ 0, 0, 1}, { 1, 0, 0}, { 0, 1, 0}, {{-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}}},  // +Z
                    {{ 0, 0,-1}, {-1, 0, 0}, { 0, 1, 0}, {{ h,-h,-h}, {-h,-h,-h}, {-h, h,-h}, { h, h,-h}}},  // -Z
                }};
            }
        }();        

        for (const Face& face : faces) {
            const unsigned int base = static_cast<unsigned int>(vertices.size());

            for (int i = 0; i < 4; i++) {
                vertices.emplace_back(face.corners[i], face.normal, corner_uvs[i], face.tangent, face.bitangent);
            }
            if (!inverted) indices.insert(indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
            else           indices.insert(indices.end(), {base, base + 2, base + 1, base, base + 3, base + 2});
        }

        // Define mesh with either a color or a texture, depending on what is given.
        if (std::holds_alternative<Texture>(tex_or_color)) {
            Texture tex = std::get<Texture>(tex_or_color);
            std::vector<Texture> textures = {tex};
            Mesh cuboid(vertices, indices, textures);
            return cuboid;
        }
        else {
            Color color = std::get<Color>(tex_or_color);
            Mesh cuboid(vertices, indices, color);
            return cuboid;
        }
        
    }
}