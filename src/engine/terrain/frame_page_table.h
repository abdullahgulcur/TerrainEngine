
#pragma once

#include "frame.h"
#include "glm.hpp"

namespace Engine {

    class FramePageTable : public Frame {
    private:

        unsigned int color;

    public:

        FramePageTable();
        FramePageTable(glm::u16vec2 size, unsigned int shaderProgramId);
        ~FramePageTable();
        void setUniforms(unsigned int color);
        void draw();
    };
}