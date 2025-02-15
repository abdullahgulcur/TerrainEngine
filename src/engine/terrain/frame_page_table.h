
#pragma once

#include "frame.h"
#include "glm.hpp"

namespace Engine {

    class FramePageTable : public Frame {
    private:

        unsigned int color;

    public:

        glm::u8vec2 mipCounts;

        FramePageTable() {}
        FramePageTable(glm::u8vec2 mipCounts);
        ~FramePageTable() {}
        void setUniforms(unsigned int color, unsigned int level);
        void draw();
    };
}