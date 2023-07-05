#include "camera.h"

#include <iostream>


namespace gpr5300
{

	void gpr5300::Camera::Update(float dt, float engineTime)
	{
		CameraPositionUpdate(dt);
		view_ = glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
	}

	void Camera::CameraPositionUpdate(float dt)
	{
		if (movements_.KeyPressed_D)
		{
			cameraPos_ += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_A)
		{
			cameraPos_ -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_E)
		{
			cameraPos_.y += cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_C)
		{
			cameraPos_.y -= cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_S)
		{
			cameraPos_ -= cameraSpeed_ * cameraFront_ * dt;
		}
		if (movements_.KeyPressed_W)
		{
			cameraPos_ += cameraSpeed_ * cameraFront_ * dt;
		}
		if (movements_.KeyPressed_RIGHT)
		{
			yaw_ += 65.0f * dt;
		}
		if (movements_.KeyPressed_LEFT)
		{
			yaw_ -= 65.0f * dt;
		}
		if (movements_.KeyPressed_UP)
		{
			pitch_ += 65.0f * dt;
		}
		if (movements_.KeyPressed_DOWN)
		{
			pitch_ -= 65.0f * dt;
		}

		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		if (pitch_ < -89.0f)
			pitch_ = -89.0f;
		glm::vec3 direction_;
		direction_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		direction_.y = sin(glm::radians(pitch_));
		direction_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		cameraFront_ = glm::normalize(direction_);
	}

	void Camera::CameraEventsKey(SDL_Event event, bool isDown)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_d:
		{
			movements_.KeyPressed_D = isDown;
			break;
		}
		case SDLK_a:
		{
			movements_.KeyPressed_A = isDown;
			break;
		}
		case SDLK_e:
		{
			movements_.KeyPressed_E = isDown;
			break;
		}
		case SDLK_c:
		{
			movements_.KeyPressed_C = isDown;
			break;
		}
		case SDLK_s:
		{
			movements_.KeyPressed_S = isDown;
			break;
		}
		case SDLK_w:
		{
			movements_.KeyPressed_W = isDown;
			break;
		}
		case SDLK_RIGHT:
		{
			movements_.KeyPressed_RIGHT = isDown;
			break;
		}
		case SDLK_LEFT:
		{
			movements_.KeyPressed_LEFT = isDown;
			break;
		}
		case SDLK_UP:
		{
			movements_.KeyPressed_UP = isDown;
			break;
		}
		case SDLK_DOWN:
		{
			movements_.KeyPressed_DOWN = isDown;
			break;
		}
		case SDLK_LSHIFT:
		{
			if (movements_.KeyPressed_Shift == false && isDown)
			{
				movements_.KeyPressed_Shift = true;
			}

			else if (movements_.KeyPressed_Shift == true && !isDown)
			{
				movements_.KeyPressed_Shift = false;
			}
			break;
		}
		case SDLK_1:
		{
			if (movements_.KeyPressed_1 == false && isDown)
			{
				movements_.KeyPressed_1 = true;
				if (movements_.KeyPressed_Shift)
				{
					if (cameraRotationSpeed_ < 16.0f)
					{
						cameraRotationSpeed_ *= 2.0f;
					}
				}
				else
				{
					if (cameraSpeed_ < 32.0f)
					{
						cameraSpeed_ *= 2.0f;
					}
				}
			}
			else if (movements_.KeyPressed_1 == true && !isDown)
			{
				movements_.KeyPressed_1 = false;
			}
			break;
		}
		case SDLK_2:
		{
			if (movements_.KeyPressed_2 == false && isDown)
			{
				movements_.KeyPressed_2 = true;
				if (movements_.KeyPressed_Shift)
				{
					if (cameraRotationSpeed_ > 2.0f)
					{
						cameraRotationSpeed_ *= 0.5f;
					}
				}
				else
				{
					if (cameraSpeed_ > 0.5f)
					{
						cameraSpeed_ *= 0.5f;
					}
				}
				std::cout << cameraRotationSpeed_ << std::endl;
			}
			else if (movements_.KeyPressed_2 == true && !isDown)
			{
				movements_.KeyPressed_2 = false;
			}
			std::cout << cameraRotationSpeed_ << std::endl;
			break;
		}
		default:
		{
			break;
		}
		}
	}

	void Camera::CameraMouseMovement(float mouseX, float mouseY, float dt)
	{
		std::cout << mouseX << " " << mouseY << std::endl;
		yaw_ += mouseX * cameraRotationSpeed_ * dt;
		pitch_ += mouseY * cameraRotationSpeed_ * dt;
	}
};