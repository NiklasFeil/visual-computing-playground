#pragma once
//#include <GLAD/glad.h>
#include "Core/Layer.hpp"
#include "Core/Event.hpp"
#include "Core/Shader.hpp"
#include <vector>
#include <memory>

class AppLayer : public Core::Layer {

    public:
        AppLayer();
        ~AppLayer();

        //void onEvent(Core::Event& event) override;
        void onUpdate(float dt) override;
        void onRender() override;

    private:
        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_shaderprogram;

        std::vector<float> m_vertices;
        std::unique_ptr<Core::Shader> m_basic_shader;
};