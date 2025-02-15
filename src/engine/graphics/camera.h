#pragma once

#include "glm.hpp"
//#include "Math/transform.h"

namespace Engine {

	class Camera {
	private:
	public:

		glm::u16vec2 bounds;
		float fov = 90.f;
		float nearClip = 0.1f;
		float farClip = 16000.f;
		float aspectRatio;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec4 planes[6];

		static Camera* create();
		Camera();
		~Camera();
		void init(glm::u16vec2 bounds);
		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setTransform(const glm::vec3& position, const glm::vec3& rotation);
		//void setTransform(const Transform2C& transform);
		//void setModelMatrix(const glm::mat4& model);
		void setViewMatrix();
		void setViewMatrix(const glm::mat4& view);
		void setProjectionMatrix();
		void setBounds(glm::u16vec2 bounds);
		void setNearClip(float nearClip);
		void setFarClip(float farClip);
		void setFov(float fov);
		//void setAspectRatio(float aspectRatio);

	};
}