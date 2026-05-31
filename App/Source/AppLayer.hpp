#pragma once
//#include <GLAD/glad.h>
#include "Core/Layer.hpp"
#include "Core/Event.hpp"
#include "Core/Shader.hpp"
#include "Core/Object.hpp"
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

        std::unique_ptr<Core::Object> m_triangle;
        std::unique_ptr<Core::Shader> m_basic_shader;
};