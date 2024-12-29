#include "pch.h"
#include "core.h"

namespace Engine {

    Core* Core::instance;

    void Core::init() {

        glfwContext = GLFWContext::create();
        input = Input::create();
        camera = Camera::create();
        shader = Shader::create();
        scene = Scene::create();
        renderer = Renderer::create();

        glfwContext->init();
        input->init();
        camera->init(glfwContext->getScreenSize());

        renderer->init();
        shader->init(); // this must be in renderer
        renderer->loadFrames();
    }

    void Core::update() {

        scene->update();
        input->update();
        glfwContext->pollEvents();
        glfwContext->swapBuffers();

        renderer->draw(scene);
    }

    Core* Core::getInstance() {
        if (!instance)
            instance = new Core;
        return instance;
    }

    GLFWContext* Core::getGlfwContext() {
        return instance->glfwContext;
    }

    Input* Core::getInput() {
        return instance->input;
    }

    Camera* Core::getCamera() {
        return instance->camera;
    }

    Shader* Core::getShader() {
        return instance->shader;
    }

}