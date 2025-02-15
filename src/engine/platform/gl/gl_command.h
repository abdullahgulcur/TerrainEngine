#pragma once

#include "GL/glew.h"
#include "glm.hpp"

namespace Engine {

    class GLCommand {
    private:
    public:

		static void refreshScene(glm::u16vec2 size, unsigned int FBO);
		static void refreshFrame(glm::u16vec2 size, unsigned int FBO);
		static void drawInstanced(unsigned int indiceCount, unsigned int instanceCount);
		static void drawUninstanced(unsigned int indiceCount);
		static void drawInstancedWithSubData(unsigned int VAO, unsigned int indiceCount, unsigned int instanceCount, unsigned int buffer, unsigned int offset, unsigned int size, void* ptr);
		static void drawInstancedWithSubData1(unsigned int VAO, unsigned int indiceCount, unsigned int instanceCount, unsigned int buffer, unsigned int offset, unsigned int size, void* ptr);
		static void drawArrays(unsigned int count);
		static void setScreen(glm::u16vec2 pos, glm::u16vec2 size, unsigned int FBO);
		//static void setScreen(glm::u16vec2 pos, glm::u16vec2 size, unsigned int FBO, unsigned int textureId, unsigned short tileId);
		static void drawQuad(unsigned int quadVAO);
    };
}