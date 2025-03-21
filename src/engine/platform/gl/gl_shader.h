#pragma once

#include "GL/glew.h"

namespace Engine {

    class GLShader {
    private:
    public:

        static unsigned int loadShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath);
        static unsigned int loadShader(std::string vertexPath, std::string fragmentPath);
        static void useProgram(unsigned int programId);
        static void deleteProgram(unsigned int programId);
    };
}