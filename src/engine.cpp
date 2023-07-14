#pragma once
#include "engine.h"

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <imgui.h>

#include <imgui_impl_opengl3.h>

#include <cassert>
#include <chrono>
#include <iostream>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace gpr5300
{
	Engine::Engine(Scene* scene) : scene_(scene)
	{
	}

	void Engine::Run()
	{
		Begin();
		bool isOpen = true;
		bool firstLoop = true;

		using Clock = std::chrono::system_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		using seconds = std::chrono::duration<float, std::ratio<1, 1>>;

		std::chrono::duration<float> secondsSinceWindowIsOpen{};

		float m_secondCounter = 0.0f;
		//These are not the real fps, just temporary
		float m_tempFps = 0.0f;
		//This float is the fps we should use
		float fps = 0.0f;

		float longestFrame = 0.0f;

		while (isOpen)
		{

			const auto freshFrameTimer = Clock::now();

			const auto dt = std::chrono::duration_cast<seconds>(freshFrameTimer - prevFrameTimer_);

			if (firstLoop)
			{
				timeToLoad_ = dt.count();
				firstLoop = false;
			}

			//records time since window is open
			secondsSinceWindowIsOpen += dt;

			//Set newer time into previous time
			prevFrameTimer_ = freshFrameTimer;

			m_tempFps++;
			if (dt.count() > longestFrame)
			{
				longestFrame = dt.count();
			}
			if (m_secondCounter <= 1) {
				m_secondCounter += dt.count();
			}
			else
			{
				//"fps" are the actual fps
				fps = m_tempFps;
				m_secondCounter = 0;
				m_tempFps = 0;
			}

			//Do something with the fps
			auto fpsc = "FPS: " + std::to_string(fps) + " Time to Load : " + std::to_string(timeToLoad_);
			const char* ooo = fpsc.c_str();
			SDL_SetWindowTitle(window_, ooo);


			int width;
			int height;
			SDL_GetWindowSize(window_, &width, &height);

			proj_ = glm::mat4(1.0f);
			proj_ = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
			view_ = glm::mat4(1.0f);
			view_ = camera_->view_;


			scene_->camProj_ = proj_;
			scene_->camView_ = view_;
			scene_->camPos_ = camera_->GetCameraPos();
			scene_->camFront_ = camera_->GetCameraFront();
			//Manage SDL event

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					isOpen = false;
					break;
				case SDL_WINDOWEVENT:
				{
					switch (event.window.event)
					{
					case SDL_WINDOWEVENT_CLOSE:
						isOpen = false;
						break;
					case SDL_WINDOWEVENT_RESIZED:
					{
						glm::uvec2 newWindowSize;
						newWindowSize.x = event.window.data1;
						newWindowSize.y = event.window.data2;
						//TODO do something with the new size
						break;
					}
					default:
					{
						break;
					}
					}
				case SDL_KEYUP:
				{
					camera_->CameraEventsKey(event, false);
					break;
				}
				case SDL_KEYDOWN:
				{
					camera_->CameraEventsKey(event, true);
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						isOpen = false;
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						rightClickOnWindow_ = true;
					}

					if (event.button.button == SDL_BUTTON_RIGHT)
					{
						SDL_SetRelativeMouseMode(SDL_TRUE);
					}
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						rightClickOnWindow_ = false;
					}
					if (event.button.button == SDL_BUTTON_RIGHT)
					{
						SDL_SetRelativeMouseMode(SDL_FALSE);
						SDL_WarpMouseInWindow(window_, width / 2, height / 2);
					}
					break;
				}
				case SDL_MOUSEMOTION:
				{
					if(SDL_GetRelativeMouseMode() || rightClickOnWindow_)
					{
						camera_->CameraMouseMovement(static_cast<float>(event.motion.xrel), -static_cast<float>(event.motion.yrel), dt.count());
					}
				}
				}
				default:

					break;
				}
				scene_->OnEvent(event);
				ImGui_ImplSDL2_ProcessEvent(&event);
			}
			glClearColor(0.2, 0.2, 0.2, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			camera_->Update(dt.count(), secondsSinceWindowIsOpen.count());
			scene_->Update(dt.count());

			//Generate new ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(window_);
			ImGui::NewFrame();

			scene_->DrawImGui();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			SDL_GL_SwapWindow(window_);

		}
		End();
	}

	void Engine::Begin()
	{
		prevFrameTimer_ = std::chrono::system_clock::now();

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
		// Set our OpenGL version.
#if true
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		constexpr auto windowSize = glm::ivec2(1440, 720);
		window_ = SDL_CreateWindow(
			"GPR5300",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			windowSize.x,
			windowSize.y,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
		);
		glRenderContext_ = SDL_GL_CreateContext(window_);
		//setting vsync
		SDL_GL_SetSwapInterval(1);

		if (GLEW_OK != glewInit())
		{
			assert(false && "Failed to initialize OpenGL context");
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Keyboard Gamepad
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();
		ImGui_ImplSDL2_InitForOpenGL(window_, glRenderContext_);
		ImGui_ImplOpenGL3_Init("#version 300 es");

		camera_ = new Camera();

		scene_->Begin();
		scene_->camPos_ = camera_->GetCameraPos();
		scene_->camFront_ = camera_->GetCameraFront();
	}

	void Engine::End()
	{
		scene_->End();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		SDL_GL_DeleteContext(glRenderContext_);
		SDL_DestroyWindow(window_);
		SDL_Quit();
	}
} // namespace gpr5300
