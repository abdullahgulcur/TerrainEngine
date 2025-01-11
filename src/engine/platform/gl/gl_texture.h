#pragma once

#include "GL/glew.h"
#include "glm.hpp"

namespace Engine {

    class GLTexture {
    private:
    public:

        static unsigned int generateTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generateTerrainPaletteTexture2D(unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generateCompressedTerrainPaletteTexture2D(std::string path);
        static unsigned int generateMaskTexture(unsigned short width, unsigned short height);
        static unsigned int generateDepthTexture(unsigned short width, unsigned short height);
        static unsigned int generateHeightmapTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generateHeightmapTexture2D(glm::u16vec2 size);
        static unsigned int generateHeightmapTexture(glm::u16vec2 size);
        static unsigned int generatePhysicalPagesTexture(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int createPhysicalPagesFrameBufferTexture(glm::u16vec2 size);
        static unsigned int createPageTableFrameBufferTexture(glm::u16vec2 size);
        static unsigned int createFrameSceneTexture(glm::u16vec2 size);
        static void updateTexture2D(unsigned int textureId, UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data, glm::ivec2 pos);
        static void updateHeightmapPhysicalTexture(unsigned int textureId, unsigned short width, unsigned short height, const unsigned int* data, glm::ivec2 pos);
        static void useTexture(unsigned int index, unsigned int textureId);
        static void setTextureUniformLocation(std::string location, unsigned int shaderProgramId, unsigned int index);
        //static void getTextureContent(UINT8 channels, unsigned int* data, unsigned int textureId);
        static void getTextureContent(UINT8 channels, unsigned char* data, unsigned int textureId);
        static void getHeightmapContent(unsigned int* data, unsigned int textureId);
        static void deleteTexture(unsigned int textureId);
        static void generateMipmap(unsigned int textureId);
    };
}