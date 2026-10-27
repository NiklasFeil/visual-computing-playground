#pragma once
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Core/Application.hpp"

namespace Core {

    class GUI {
        public:
            static void InitGui() {
                
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

                GLFWwindow* window_handle = Core::Application::Get().getWindow()->getHandle();
                ImGui_ImplGlfw_InitForOpenGL(window_handle, true);
                ImGui_ImplOpenGL3_Init();

                ImGuiStyle& style = ImGui::GetStyle();
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    style.WindowRounding = 0.0f;
                    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
                }
            }

            static void StartFrame() {
                
                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                static bool show = true;
                ImGui::ShowDemoWindow(&show);
            }

            static void Render() {
                
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

            static void Cleanup() {
                
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
                
            }
    };
}