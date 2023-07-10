#pragma once

#include <SDL.h>
#include <glm/matrix.hpp>

namespace gpr5300
{
    
    class Scene
    {
    public:
        virtual ~Scene() = default;
        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void Update(float dt) = 0;
        virtual void DrawImGui() {}
        virtual void OnEvent(const SDL_Event& event) {}

        glm::mat4 camProj_ = glm::mat4(1.0f);
        glm::mat4 camView_ = glm::mat4(1.0f);
        glm::vec3 camPos_ = glm::vec3(1.0f);
    };

} // namespace gpr5300
