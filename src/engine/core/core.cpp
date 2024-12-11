#include "pch.h"
#include "core.h"
#include "gl_context.h"

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

        glfwContext = GLFWContext::create();
        input = Input::create();
        camera = Camera::create();
        shader = Shader::create();
        terrain = Terrain::create();

        glfwContext->init();
        GLContext::init();
        input->init();
        camera->init(glfwContext->getScreenSize());
        shader->init();
        //terrain->init("../../../resource/texture/heightmap.png", static_cast<unsigned short>(Engine::TerrainClipmapSize::BLOCK_SIZE_32), 4);
    }

    void Core::update() {

        terrain->update();
        input->update();
        glfwContext->pollEvents();
        glfwContext->swapBuffers();
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