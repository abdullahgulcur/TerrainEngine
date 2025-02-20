#pragma once

#include "GL/glew.h"
#include "glm.hpp"

namespace Engine {

    class GLTexture {

        struct DDSHeader {
            uint32_t size;
            uint32_t flags;
            uint32_t height;
            uint32_t width;
            uint32_t pitchOrLinearSize;
            uint32_t depth;
            uint32_t mipMapCount;
            uint32_t reserved1[11];
            uint32_t pixelFormatSize;
            uint32_t pixelFormatFlags;
            uint32_t fourCC;
            uint32_t rgbBitCount;
            uint32_t rBitMask;
            uint32_t gBitMask;
            uint32_t bBitMask;
            uint32_t aBitMask;
            uint32_t caps;
            uint32_t caps2;
            uint32_t caps3;
            uint32_t caps4;
            uint32_t reserved2;
        };

    private:
    public:

        static unsigned int generateTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generateMacroVariationTexture(std::string path);
        static unsigned int generateTerrainPaletteTexture2D(unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generateCompressedTerrainPaletteTexture2D(std::string path);
        static unsigned int generateCompressedTerrainPaletteTextureArray(std::vector<std::string>& texturePathList, unsigned short textureSize);
        static void compressedTextureSubImage(std::string& texturePath, UINT8 index);
        static unsigned int generateMaskTexture(unsigned short width, unsigned short height);
        static unsigned int generateDepthTexture(unsigned short width, unsigned short height);
        static unsigned int generateHeightmapTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int generateHeightmapTexture2D(glm::u16vec2 size);
        static unsigned int generateHeightmapTexture(glm::u16vec2 size);
        static unsigned int generatePhysicalPagesTexture(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data);
        static unsigned int createPhysicalPagesFrameBufferTexture(glm::u16vec2 size);
        static unsigned int createPhysicalPagesFrameBufferTextureArray(unsigned short pageSize, UINT8 mipLevels, unsigned short pageCount);
        static unsigned int createPageTableFrameBufferTexture(glm::u16vec2 size);
        static unsigned int createFrameSceneTexture(glm::u16vec2 size);
        static void updateTexture2D(unsigned int textureId, UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data, glm::ivec2 pos);
        static void updateHeightmapPhysicalTexture(unsigned int textureId, unsigned short width, unsigned short height, const unsigned int* data, glm::ivec2 pos);
        static void useTexture(unsigned int index, unsigned int textureId);
        static void useTextureArray(unsigned int index, unsigned int textureId);
        static void setTextureUniformLocation(std::string location, unsigned int shaderProgramId, unsigned int index);
        //static void getTextureContent(UINT8 channels, unsigned int* data, unsigned int textureId);
        static void getTextureContent(UINT8 channels, unsigned char* data, unsigned int textureId);
        static void getHeightmapContent(unsigned int* data, unsigned int textureId);
        static void deleteTexture(unsigned int textureId);
        static void generateMipmap(unsigned int textureId);
    };
}