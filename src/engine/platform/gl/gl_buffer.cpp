#include "pch.h"
#include "gl_buffer.h"

namespace Engine {

    unsigned int GLBuffer::createVAO() {

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        return VAO;
    }

    unsigned int GLBuffer::createVBO(unsigned int size, const void* ptr) {

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW);
        return VBO;
    }

    unsigned int GLBuffer::createEBO(unsigned int size, const void* ptr) {
        
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW);
        return EBO;
    }

    void GLBuffer::setInstancedVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer) {

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
        glVertexAttribDivisor(index, 1);
    }

    void GLBuffer::setVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer) {

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
    }

    /*InstancedStaticMesh GLBuffer::createInstancedTerrainStaticMesh2D(void* vertices, unsigned int sizeVertices, void* indices, unsigned int sizeIndices, unsigned int instanceCount) {

        unsigned int VAO = GLBuffer::createVAO();
        GLBuffer::createVBO(sizeVertices, vertices);
        GLBuffer::setVertexAttribute(0, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);

        GLBuffer::createEBO(sizeIndices, indices);

        int stride = 12;
        unsigned int instanceBuffer = GLBuffer::createVBO(instanceCount * stride, NULL);

        int counter = 0;
        GLBuffer::setInstancedVertexAttribute(1, 2, GL_FLOAT, stride, (void*)counter);
        GLBuffer::setInstancedVertexAttribute(2, 1, GL_FLOAT, stride, (void*)(counter += sizeof(glm::vec2)));

        glBindVertexArray(0);

        return InstancedStaticMesh(VAO, indices.size(), instanceBuffer);
    }*/
}