#pragma once

#include "frame.h"
#include "glm.hpp"

namespace Engine {

    class FramePhsyicalPages : public Frame {
    private:

        unsigned int level;
        glm::vec2 position;
        unsigned int grassTextureId;

    public:

        FramePhsyicalPages();
        FramePhsyicalPages(glm::u16vec2 size, unsigned int grassTextureId, unsigned int shaderProgramId);
        ~FramePhsyicalPages();
        void setUniforms(unsigned int level, glm::vec2 position);
        void draw();
    };

}