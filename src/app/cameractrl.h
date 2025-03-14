#pragma once

#include "glm.hpp"

namespace Game {

	class CameraController {
	private:

		float rotationSpeed = 0.01;
		float scrollSpeed = 20;
		float horizontalAngle = 0.f;
		float verticalAngle = 0.f;
		float speed = 1.f;
		float smoothSpeed = 0.15f;
		glm::vec3 position = glm::vec3(4078, 300, 4078);
		glm::vec3 targetPosition = glm::vec3(4078, 300, 4078);

	public:

		CameraController();
		~CameraController();
		void start();
		void update();
	};
}