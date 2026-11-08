#include "Gui.hpp"
#include "Logger/Logger.hpp"
#include "ExceptionHandler/ExceptionHandler.hpp"
#include <print>
#include <format>

namespace Core {

    static Gui* s_Gui = nullptr;

    Gui::Gui(const GuiConfig& gui_config)
    : m_gui_config(gui_config) {}

    void Gui::create(Window& win) {
        s_Gui = this;
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        GLFWwindow* window_handle = win.getHandle();
        ImGui_ImplGlfw_InitForOpenGL(window_handle, true);
        ImGui_ImplOpenGL3_Init();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
    }

    Gui::~Gui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        s_Gui = nullptr;
    }

    void Gui::render() {

        // Start the Dear ImGui frame
        //std::cout << "Render gui" << std::endl;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for(auto& win : m_gui_windows) {
            win.second.render();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void Gui::raiseEvent(Event& event) {
        if (m_gui_config.event_callback)
            m_gui_config.event_callback(event);
    }

    GuiWindow& Gui::createGuiWindow(std::string title) {
        GuiWindowSpecs specs;
        specs.title = std::move(title);
        specs.event_callback = [this](Event& event) { raiseEvent(event); };
        auto pair = m_gui_windows.emplace(specs.title, specs);
        auto it = pair.first;
        bool inserted = pair.second;
        ExceptionHandler::Assert(inserted, std::format("Creating GuiWindow {} failed!", title));
        return (*it).second;
    }

    void Gui::destroyGuiWindow(const std::string& title) {
        unsigned int num_removed = m_gui_windows.erase(title);
        assert(num_removed);
        std::println("GuiWindow {} destroyed!", title);
    }

    GuiWindow& Gui::getGuiWindow(const std::string& title) {
        return m_gui_windows.at(title);
    }
}