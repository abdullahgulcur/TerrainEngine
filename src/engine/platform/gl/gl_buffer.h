#pragma once

#include "GL/glew.h"
#include "glm.hpp"

namespace Engine {

    class GLBuffer {
    private:
    public:

        static unsigned int createVAO();
        static void bindVAO(unsigned int VAO);
        static void deleteVAO(unsigned int& VAO);
        static void deleteBuffer(unsigned int& buffer);
        static unsigned int createVBO(unsigned int size, const void* ptr);
        static unsigned int createEBO(unsigned int size, const void* ptr);
        static void setInstancedVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer);
        static void setVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer);
        static unsigned int createQuadVAO();
        static void setArrayBufferSubData(unsigned int buffer, unsigned int offset, unsigned int size, void* ptr);
        static void createInstancedTerrainStaticMesh2D(unsigned int& VAO, unsigned int& instanceBuffer, void* vertices, unsigned int sizeVertices, void* indices, unsigned int sizeIndices, unsigned int instanceCount);
        static unsigned int createFBO(unsigned int textureId);

    };
}