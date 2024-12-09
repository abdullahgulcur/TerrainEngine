#pragma once

#include "GL/glew.h"

namespace Engine {

    class GLShader {
    private:
    public:

        static unsigned int loadShader(std::string vertexPath, std::string fragmentPath);
        static void useProgram(unsigned int programId);
    };
}