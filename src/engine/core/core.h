#pragma once

#include "glfw_context.h"
#include "input.h"
#include "camera.h"
#include "shader.h"
#include "renderer.h"
#include "scene.h"

namespace Engine {

    class Core {
    private:

        static Core* instance;

        GLFWContext* glfwContext = NULL;
        Input* input = NULL;
        Camera* camera = NULL;
        Shader* shader = NULL;
        Renderer* renderer = NULL;

        Core() {}
        ~Core() {}

    public:

        Scene* scene = NULL;

        void init();
        void update();

        static Core* getInstance();
        static GLFWContext* getGlfwContext();
        static Input* getInput();
        static Camera* getCamera();
        static Shader* getShader();
    };
}