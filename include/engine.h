#pragma once
#include <chrono>

#include "scene.h"
#include "camera.h"

#include <imgui_impl_sdl2.h>

namespace gpr5300
{

class Engine
{
public:
    Engine(Scene* scene);
    void Run();
private:
    void Begin();
    void End();
    Scene* scene_ = nullptr;
    SDL_Window* window_ = nullptr;
    Camera* camera_;
    SDL_GLContext glRenderContext_{};
    glm::mat4 proj_ = glm::mat4(1.0f);
    glm::mat4 view_ = glm::mat4(1.0f);
    std::chrono::time_point<std::chrono::system_clock> prevFrameTimer_;
    float timeToLoad_;
    bool rightClickOnWindow_ = false;
};
    
} // namespace gpr5300
