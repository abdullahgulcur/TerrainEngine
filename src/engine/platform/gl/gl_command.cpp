#include "pch.h"
#include "gl_command.h"
#include "gl_buffer.h"

namespace Engine {

	//void GLCommand::refreshScene(glm::u16vec2 size, unsigned int FBO) {

	//	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//	glViewport(0, 0, size.x, size.y);

	//	//glClearColor(0.f, 0.f, 0.f, 1.f);
	//	glClearColor(0.2f, 0.2f, 0.2f, 1.f);

	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glEnable(GL_DEPTH_TEST);
	//}

	void GLCommand::refreshScene(glm::u16vec2 size, unsigned int FBO) {

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, size.x, size.y);

		//glClearColor(0.f, 0.f, 0.f, 1.f);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	void GLCommand::refreshFrame(glm::u16vec2 size, unsigned int FBO) {

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, size.x, size.y);

		//glClearColor(0.f, 0.f, 0.f, 1.f);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);

		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
	}

	void GLCommand::drawInstanced(unsigned int indiceCount, unsigned int instanceCount) {

		glDrawElementsInstanced(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, (void*)0, instanceCount);
	}

    void GLCommand::drawUninstanced(unsigned int indiceCount) {

        glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, (void*)0);
    }

    void GLCommand::drawInstancedWithSubData(unsigned int VAO, unsigned int indiceCount, unsigned int instanceCount, unsigned int buffer, unsigned int offset, unsigned int size, void* ptr) {

		GLBuffer::setArrayBufferSubData(buffer, offset, size, ptr);
		GLBuffer::bindVAO(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, (void*)0, instanceCount);
    }

	void GLCommand::drawInstancedWithSubData1(unsigned int VAO, unsigned int indiceCount, unsigned int instanceCount, unsigned int buffer, unsigned int offset, unsigned int size, void* ptr) {

		GLBuffer::setArrayBufferSubData(buffer, offset, size, ptr);
		GLBuffer::bindVAO(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, indiceCount, GL_UNSIGNED_SHORT, (void*)0, instanceCount);
	}

	void GLCommand::drawArrays(unsigned int count) {

		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void GLCommand::setScreen(glm::u16vec2 pos, glm::u16vec2 size, unsigned int FBO) {

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(pos.x, pos.y, size.x, size.y);
	}

	void GLCommand::drawQuad(unsigned int quadVAO) {

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

}