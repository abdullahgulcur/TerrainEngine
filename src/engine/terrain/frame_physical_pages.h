#pragma once

#include "frame.h"
#include "glm.hpp"

namespace Engine {

    class FramePhsyicalPages {
    private:

        unsigned int shaderProgramId;
        unsigned int planeVAO;
        unsigned int FBO;

        glm::u16vec2 viewportPos;
        glm::u16vec2 viewportSize;

        unsigned int level;
        unsigned int blockSize;
        glm::u16vec2 blockPosition;
        glm::u8vec2 pagePosition;
        glm::u8vec2 physicalTextureSize;

        unsigned int textureIdList[9];

    public:

        glm::u16vec2 size;

        unsigned int texture_0_Id; // multiple textureId for albedo, normal, displacement, mask etc...
        /*unsigned int texture_1_Id;
        unsigned int texture_2_Id;*/

        FramePhsyicalPages();
        FramePhsyicalPages(glm::u16vec2 size, unsigned int heightmapTextureId, unsigned int blockSize);
        ~FramePhsyicalPages();
        void setUniforms(unsigned int level, glm::u16vec2 blockPosition, glm::u8vec2 pagePosition, glm::u8vec2 physicalTextureSize);
        void draw();
        void setViewport(glm::u16vec2 pos, glm::u16vec2 size);
    };

}