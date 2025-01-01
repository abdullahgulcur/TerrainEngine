#pragma once

#include "frame.h"
#include "glm.hpp"

namespace Engine {

    class FramePhsyicalPages : public Frame {
    private:

        unsigned int level;
        glm::vec2 position;
        glm::u8vec2 pagePosition;
        glm::u8vec2 physicalTextureSize;

        unsigned int heightmapTextureId;
        unsigned int tex_0_TextureId;
        unsigned int tex_1_TextureId;
        unsigned int tex_2_TextureId;
        //unsigned int perlinImageTextureId;

    public:

        FramePhsyicalPages();
        FramePhsyicalPages(glm::u16vec2 size, unsigned int heightmapTextureId);
        ~FramePhsyicalPages();
        void setUniforms(unsigned int level, glm::vec2 position, glm::u8vec2 pagePosition, glm::u8vec2 physicalTextureSize);
        void draw();
    };

}