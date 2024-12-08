#include "pch.h"
#include "camera.h"
#include "transform.h"
#include "ext/matrix_clip_space.hpp"

namespace Engine {

	Camera* Camera::create() {
		
		return new Camera;
	}

	Camera::Camera() {}

	Camera::~Camera() {}

	void Camera::init(glm::u16vec2 bounds) {

		Camera::setBounds(bounds);
	}

	void Camera::setPosition(const glm::vec3& position) {

		this->position = position;
		Camera::setViewMatrix();
	}

	void Camera::setRotation(const glm::vec3& rotation) {

		this->rotation = rotation;
		Camera::setViewMatrix();
	}

	void Camera::setTransform(const glm::vec3& position, const glm::vec3& rotation) {

		this->position = position;
		this->rotation = rotation;
		Camera::setViewMatrix();
	}

	/*void Camera::setTransform(const Transform2C& transform) {

		this->transform = transform;
		Camera::setViewMatrix();
	}*/

	void Camera::setBounds(glm::u16vec2 bounds) {

		this->bounds = bounds;
		this->aspectRatio = float(bounds.x) / bounds.y;
		Camera::setProjectionMatrix();
	}

	void Camera::setNearClip(float nearClip) {

		this->nearClip = nearClip;
		Camera::setProjectionMatrix();
	}

	void Camera::setFarClip(float farClip) {

		this->farClip = farClip;
		Camera::setProjectionMatrix();
	}

	void Camera::setFov(float fov) {

		this->fov = fov;
		Camera::setProjectionMatrix();
	}

	void Camera::setProjectionMatrix() {

		this->projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	}

	//void Camera::setModelMatrix(const glm::mat4& model) {

	//	this->viewMatrix = Transform::getViewMatrix(model);
	//}

	void Camera::setViewMatrix() {
		Transform transform(this->position, this->rotation);
		this->viewMatrix = transform.getViewMatrix();
	}

	void Camera::setViewMatrix(const glm::mat4& view) {

		Transform transform;
		transform.decomposeModelMatrixNoScale(glm::inverse(view));
		this->position = transform.position;
		this->rotation = transform.rotation;
		this->viewMatrix = view;

	}

}