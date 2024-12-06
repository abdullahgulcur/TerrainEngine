#include "pch.h"
#include "core.h"

namespace Engine {

    Core* Core::instance;

    Core* Core::getInstance() {
        if (!instance)
            instance = new Core;
        return instance;
    }

    Core::Core() {}

    Core::~Core() {}

    void Core::init() {

        glfwContext = new GLFWContext;

        glfwContext->init();
    }

    void Core::update() {

        glfwContext->pollEvents();
        glfwContext->swapBuffers();
    }

    GLFWContext* Core::getGlfwContext() {

        return instance->glfwContext;
    }
}