#include "pch.h"
#include "core.h"
#include "cameractrl.h"
#include "ext/matrix_transform.hpp"

namespace Game {

	CameraController::CameraController() {}

	CameraController::~CameraController() {}

	void CameraController::start() {

		Engine::Camera* camera = Engine::Core::getCamera();
		glm::vec3 direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
		glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
		glm::vec3 up = glm::cross(right, direction);
		glm::mat4 model = glm::lookAt(targetPosition, targetPosition + direction, up);
		camera->setViewMatrix(model);
	}

	void CameraController::update() {

		Engine::Camera* camera = Engine::Core::getCamera();
		Engine::Input* input = Engine::Core::getInput();
		Engine::GLFWContext* glfwContext = Engine::Core::getGlfwContext();

		if (input->getMousePressed(MOUSE_BUTTON_2)) {

			glfwContext->setCursorVisible(false);

			glm::vec2 mouseDelta = input->getMouseDelta();
			horizontalAngle -= rotationSpeed * mouseDelta.x;
			verticalAngle -= rotationSpeed * mouseDelta.y;

			glm::vec3 direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
			glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
			glm::vec3 up = glm::cross(right, direction);

			if (input->getKeyPressed(KEY_W))
				targetPosition += direction * 0.1f * speed;

			if (input->getKeyPressed(KEY_S))
				targetPosition -= direction * 0.1f * speed;

			if (input->getKeyPressed(KEY_A))
				targetPosition -= right * 0.1f * speed;

			if (input->getKeyPressed(KEY_D))
				targetPosition += right * 0.1f * speed;

			if (input->getKeyPressed(KEY_Q))
				targetPosition -= up * 0.1f * speed;

			if (input->getKeyPressed(KEY_E))
				targetPosition += up * 0.1f * speed;

			position = glm::mix(position, targetPosition, smoothSpeed);
			glm::mat4 model = glm::lookAt(position, position + direction, up);
			camera->setPosition(position);
			camera->setViewMatrix(model);

			speed += input->getMouseScrollDelta() * 0.1f;
			speed = glm::clamp(speed, 0.003f, 30.f);
		}

		if (input->getMouseUp(MOUSE_BUTTON_2))
			glfwContext->setCursorVisible(true);
	}

}