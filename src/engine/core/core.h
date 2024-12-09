#pragma once

#include "glfw_context.h"
#include "input.h"
#include "camera.h"
#include "shader.h"

namespace Engine {

    class Core {
    private:

        static Core* instance;

        GLFWContext* glfwContext;
        Input* input;
        Camera* camera;
        Shader* shader;

        Core();
        ~Core();

    public:
        
        static Core* getInstance();

        void init();
        void update();

        static GLFWContext* getGlfwContext();
        static Input* getInput();
        static Camera* getCamera();
        static Shader* getShader();
    };
}