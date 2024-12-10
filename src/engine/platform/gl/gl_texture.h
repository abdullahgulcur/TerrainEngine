#pragma once

#include "GL/glew.h"
#include "glm.hpp"

namespace Engine {

    class GLTexture {
    private:
    public:

        static unsigned int generateTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generatePhysicalPagesTexture(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int createPhysicalPagesFrameBufferTexture(glm::u16vec2 size);
        static unsigned int createPageTableFrameBufferTexture(glm::u16vec2 size);
        static void updateTexture2D(unsigned int textureId, UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data, glm::ivec2 pos);
        static void useTexture(unsigned int index, unsigned int textureId);
        static void setTextureUniformLocation(std::string location, unsigned int shaderProgramId, unsigned int index);
    };
}