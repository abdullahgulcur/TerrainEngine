#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"

namespace Engine {

	class GLFWContext {
	private:

		GLFWwindow* GLFW_window;
		GLFWmonitor* monitor;
		const GLFWvidmode* mode;

	public:

		GLFWContext();
		~GLFWContext();
		void init();
		bool shouldClose();
		void pollEvents();
		void swapBuffers();
		void destroy();
		void setTitle(const char* title);
		void setCursorPos(glm::u16vec2 cursorPos);
		glm::u16vec2 getCursorPos();
		float getTime();
		void setCursorVisible(bool visible);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	};
}