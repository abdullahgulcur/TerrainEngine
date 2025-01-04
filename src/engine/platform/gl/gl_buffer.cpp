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

    void GLBuffer::setInstancedIntegerVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer) {

        glEnableVertexAttribArray(index);
        glVertexAttribIPointer(index, size, type, stride, pointer);
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

    void GLBuffer::setIntegerVertexAttribute(unsigned int index, unsigned int size, unsigned int type, int stride, const void* pointer) {

        glEnableVertexAttribArray(index);
        glVertexAttribIPointer(index, size, type, stride, pointer);
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
        GLBuffer::setIntegerVertexAttribute(0, 2, GL_UNSIGNED_SHORT, sizeof(glm::u16vec2), (void*)0);

        GLBuffer::createEBO(sizeIndices, indices);

        int stride = 6;
        instanceBuffer = GLBuffer::createVBO(instanceCount * stride, NULL);

        GLBuffer::setInstancedIntegerVertexAttribute(1, 2, GL_UNSIGNED_SHORT, stride, (void*)0);
        GLBuffer::setInstancedIntegerVertexAttribute(2, 1, GL_UNSIGNED_BYTE, stride, (void*)sizeof(glm::u16vec2));

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

    unsigned int GLBuffer::createFBO(unsigned int textureId, unsigned int depthTextureId) {

        unsigned int FBO;
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        //glBindTexture(GL_TEXTURE_2D, textureId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

        //glBindTexture(GL_TEXTURE_2D, depthTextureId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);

        //glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer not complete" << std::endl;

        return FBO;
    }

    unsigned int GLBuffer::createRBO(glm::u16vec2 size) {

        unsigned int RBO;
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        return RBO;
    }
}