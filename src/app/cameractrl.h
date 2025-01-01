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
		float smoothSpeed = 0.05f;
		glm::vec3 position = glm::vec3(2037, 150, 2037);
		glm::vec3 targetPosition = glm::vec3(2037, 150, 2037);

	public:

		CameraController();
		~CameraController();
		void start();
		void update();
	};
}