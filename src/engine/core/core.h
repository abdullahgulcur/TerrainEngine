#pragma once

#include "glfw_context.h"

namespace Engine {

    class Core {
    private:

        static Core* instance;

        GLFWContext* glfwContext;

        Core();
        ~Core();

    public:
        
        static Core* getInstance();

        void init();
        void update();

        static GLFWContext* getGlfwContext();
    };
}