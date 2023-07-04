#pragma once
#include "scene.h"
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
    SDL_GLContext glRenderContext_{};
    glm::mat4 proj_ = glm::mat4(1.0f);
    glm::mat4 view_ = glm::mat4(1.0f);
};
    
} // namespace gpr5300
