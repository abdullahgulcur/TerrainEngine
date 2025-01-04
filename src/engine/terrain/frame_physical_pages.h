#pragma once

#include "frame.h"
#include "glm.hpp"

namespace Engine {

    class FramePhsyicalPages : public Frame {
    private:

        unsigned int level;
        unsigned int blockSize;
        glm::u16vec2 blockPosition;
        glm::u8vec2 pagePosition;
        glm::u8vec2 physicalTextureSize;

        unsigned int heightmapTextureId;
        unsigned int tex_0_TextureId;
        unsigned int tex_1_TextureId;
        unsigned int tex_2_TextureId;
        unsigned int tex_3_TextureId;
        unsigned int tex_4_TextureId;
        //unsigned int perlinImageTextureId;

    public:

        FramePhsyicalPages();
        FramePhsyicalPages(glm::u16vec2 size, unsigned int heightmapTextureId, unsigned int blockSize);
        ~FramePhsyicalPages();
        void setUniforms(unsigned int level, glm::u16vec2 blockPosition, glm::u8vec2 pagePosition, glm::u8vec2 physicalTextureSize);
        void draw();
    };

}