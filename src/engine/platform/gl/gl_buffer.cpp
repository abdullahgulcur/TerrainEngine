#include "pch.h"
#include "gl_buffer.h"

namespace Engine {

    unsigned int GLBuffer::createVAO() {

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        return VAO;
    }

    void GLBuffer::bindVAO(unsigned int VAO) {

        glBindVertexArray(VAO);
    }

    void GLBuffer::deleteVAO(unsigned int& VAO) {

        glDeleteVertexArrays(1, &VAO);
    }

    void GLBuffer::deleteBuffer(unsigned int& buffer) {

        glDeleteBuffers(1, &buffer);
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

    void GLBuffer::setArrayBufferSubData(unsigned int buffer, unsigned int offset, unsigned int size, void* ptr) {

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, ptr);
    }


    void GLBuffer::setVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer) {

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
    }

	unsigned int GLBuffer::createQuadVAO() {

		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

        unsigned int quadVAO = GLBuffer::createVAO();
        unsigned int quadVBO = GLBuffer::createVBO(sizeof(quadVertices), quadVertices);
        GLBuffer::setVertexAttribute(0, 2, GL_FLOAT, 4 * sizeof(float), 0);
        GLBuffer::setVertexAttribute(1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		return quadVAO;
	}

    void GLBuffer::createInstancedTerrainStaticMesh2D(unsigned int& VAO, unsigned int& instanceBuffer, void* vertices, unsigned int sizeVertices, void* indices, unsigned int sizeIndices, unsigned int instanceCount) {

        VAO = GLBuffer::createVAO();
        GLBuffer::createVBO(sizeVertices, vertices);
        GLBuffer::setVertexAttribute(0, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);

        GLBuffer::createEBO(sizeIndices, indices);

        int stride = 12;
        instanceBuffer = GLBuffer::createVBO(instanceCount * stride, NULL);

        int counter = 0;
        GLBuffer::setInstancedVertexAttribute(1, 2, GL_FLOAT, stride, (void*)counter);
        GLBuffer::setInstancedVertexAttribute(2, 1, GL_FLOAT, stride, (void*)(counter += sizeof(glm::vec2)));

        glBindVertexArray(0);
    }

    unsigned int GLBuffer::createFBO(unsigned int textureId) {

        unsigned int FBO;
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer not complete" << std::endl;

        return FBO;
    }
}