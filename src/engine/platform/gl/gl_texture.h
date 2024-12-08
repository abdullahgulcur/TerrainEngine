#pragma once

#include "GL/glew.h"

namespace Engine {

    class GLTexture {
    private:
    public:

        static unsigned int generateTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generatePhysicalPagesTexture(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static void useTexture(unsigned int index, unsigned int textureId);
        static void setTextureUniformLocation(std::string location, unsigned int shaderProgramId, unsigned int index);
    };
}