#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>

#include "SDL_events.h"


namespace gpr5300
{
	class CameraMovements
	{
	public:
		bool KeyPressed_D = false;
		bool KeyPressed_A = false;
		bool KeyPressed_E = false;
		bool KeyPressed_C = false;
		bool KeyPressed_S = false;
		bool KeyPressed_W = false;
		bool KeyPressed_RIGHT = false;
		bool KeyPressed_LEFT = false;
		bool KeyPressed_UP = false;
		bool KeyPressed_DOWN = false;
		bool KeyPressed_1 = false;
		bool KeyPressed_2 = false;
		bool KeyPressed_Shift = false;
	};
	
	class Camera
	{
	private:
		CameraMovements movements_;
		float cameraSpeed_ = 1.0f;
		glm::vec3 cameraPos_ = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget_ = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraFront_ = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraInverseDirection_ = glm::normalize(cameraPos_ - cameraTarget_);
		glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight_ = glm::normalize(glm::cross(up_, cameraInverseDirection_));
		glm::vec3 cameraUp_ = glm::normalize(glm::cross(cameraInverseDirection_, cameraRight_));
		float yaw_ = 275.0f;
		float pitch_ = 0.0f;
		float cameraRotationSpeed_ = 4.0f;

	public:
		glm::mat4 view_ = glm::mat4(0.0f);
		
		void Update(float dt, float engineTime);
		void CameraPositionUpdate(float dt);
		void CameraEventsKey(SDL_Event event, bool isDown);
		void CameraMouseMovement(float mouseX, float mouseY, float dt);
		glm::vec3 GetCameraPos() { return cameraPos_; }
		glm::vec3 GetCameraFront() { return cameraFront_; }
	};
};