#pragma once

#include "frame.h"
#include "glm.hpp"

namespace Engine {

    class FramePhsyicalPages {
    private:

        unsigned int shaderProgramId;
        unsigned int shaderProgramId1;
        unsigned int planeVAO;
        unsigned int FBO;

        //glm::u16vec2 viewportPos;
        unsigned short tileIndex;
        //glm::u16vec2 viewportSize;

        unsigned int level;
        glm::u16vec2 blockPosition;
        //glm::u8vec2 pagePosition;
        unsigned short pageSize;

        unsigned int textureIdList[4];
        unsigned int texturePaletteTextureArrayId;

    public:

        //glm::u16vec2 size;
        unsigned short pageCounts;

        unsigned int textureId;
        //unsigned int blockSize;

        FramePhsyicalPages() {}
        ~FramePhsyicalPages() {}
        FramePhsyicalPages(unsigned short pageCounts, unsigned short pageSize, unsigned int heightmapTextureId, unsigned int shadowmapTextureId);
        void setUniforms(unsigned int level, glm::u16vec2 blockPosition);
        void draw(unsigned short tileIndex, UINT8 mipmapLevel);
        //void setViewport(glm::u16vec2 size);
    };

}