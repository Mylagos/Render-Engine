#include "camera.h"

#include <iostream>


namespace gpr5300
{

	void gpr5300::Camera::Update(float dt, float engineTime)
	{
		CameraPositionUpdate(dt);
		view_ = glm::lookAt(CameraPos, CameraPos + cameraFront_, cameraUp_);
	}

	void Camera::CameraPositionUpdate(float dt)
	{
		if (movements_.KeyPressed_D)
		{
			CameraPos += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_A)
		{
			CameraPos -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_E)
		{
			CameraPos.y += cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_C)
		{
			CameraPos.y -= cameraSpeed_ * dt;
		}
		if (movements_.KeyPressed_S)
		{
			CameraPos -= cameraSpeed_ * cameraFront_ * dt;
		}
		if (movements_.KeyPressed_W)
		{
			CameraPos += cameraSpeed_ * cameraFront_ * dt;
		}
		if (movements_.KeyPressed_RIGHT)
		{
			Yaw += 65.0f * dt;
		}
		if (movements_.KeyPressed_LEFT)
		{
			Yaw -= 65.0f * dt;
		}
		if (movements_.KeyPressed_UP)
		{
			Pitch += 65.0f * dt;
		}
		if (movements_.KeyPressed_DOWN)
		{
			Pitch -= 65.0f * dt;
		}

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
		glm::vec3 direction;
		direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		direction.y = sin(glm::radians(Pitch));
		direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraFront_ = glm::normalize(direction);
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
			}
			else if (movements_.KeyPressed_2 == true && !isDown)
			{
				movements_.KeyPressed_2 = false;
			}
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
		Yaw += mouseX * cameraRotationSpeed_ * dt;
		Pitch += mouseY * cameraRotationSpeed_ * dt;
	}
};