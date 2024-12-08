#pragma once

#include "GL/glew.h"
#include "glm.hpp"

namespace Engine {

    class GLBuffer {
    private:
    public:

        static unsigned int createVAO();
        static unsigned int createVBO(unsigned int size, const void* ptr);
        static unsigned int createEBO(unsigned int size, const void* ptr);
        static void setInstancedVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer);
        static void setVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer);
    };
}