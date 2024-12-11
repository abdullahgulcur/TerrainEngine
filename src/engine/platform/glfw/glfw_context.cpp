#include "pch.h"
#include "glfw_context.h"
#include "core.h"

namespace Engine {

	GLFWContext* GLFWContext::create() {

		return new GLFWContext();
	}

	GLFWContext::GLFWContext() {}

	GLFWContext::~GLFWContext() {}

	void GLFWContext::init() {

		if (!glfwInit())
			fprintf(stderr, "Failed to initialize GLFW\n");

		//glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);

		GLFW_window = glfwCreateWindow(mode->width, mode->height, "Application", NULL, NULL); // windowed
		//GLFW_window = glfwCreateWindow(mode->width, mode->height, "Application", monitor, NULL); // fullscreen

		glfwMaximizeWindow(GLFW_window);
		glfwMakeContextCurrent(GLFW_window);

		glfwSetInputMode(GLFW_window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetInputMode(GLFW_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		glfwSetKeyCallback(GLFW_window, key_callback);
		glfwSetMouseButtonCallback(GLFW_window, mouseButtonCallback);
		glfwSetScrollCallback(GLFW_window, mouseScrollCallback);

		// TODO:
		//Core::getInstance()->screenWidth = mode->width;
		//Core::getInstance()->screenHeight = mode->height;
	}

	bool GLFWContext::shouldClose() {

		return glfwGetKey(GLFW_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(GLFW_window);
	}

	void GLFWContext::pollEvents() {

		glfwPollEvents();
	}

	void GLFWContext::swapBuffers() {

		glfwSwapBuffers(GLFW_window);
	}

	void GLFWContext::destroy() {

		if (GLFW_window) {
			glfwDestroyWindow(GLFW_window);
			GLFW_window = nullptr;
		}
		glfwTerminate();
	}

	void GLFWContext::setTitle(const char* title) {

		glfwSetWindowTitle(GLFW_window, title);
	}

	void GLFWContext::setCursorPos(glm::u16vec2 cursorPos) {

		glfwSetCursorPos(GLFW_window, cursorPos.x, cursorPos.y);
	}

	glm::u16vec2 GLFWContext::getCursorPos() {

		double xPos, yPos;
		glfwGetCursorPos(GLFW_window, &xPos, &yPos);
		return glm::u16vec2(xPos, yPos);
	}

	glm::u16vec2 GLFWContext::getScreenSize() {

		return glm::u16vec2(this->mode->width, this->mode->height);
	}

	float GLFWContext::getTime() {

		return (float)glfwGetTime();
	}

	void GLFWContext::setCursorVisible(bool visible) {

		int value = visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN;
		glfwSetInputMode(GLFW_window, GLFW_CURSOR, value);
	}

	void GLFWContext::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		Core::getInput()->handleKeyboardInput(key, action);
	}

	void GLFWContext::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

		Core::getInput()->handleMouseInput(button, action);
	}

	void GLFWContext::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

		Core::getInput()->handleMouseScrollInput(yoffset);
	}

}