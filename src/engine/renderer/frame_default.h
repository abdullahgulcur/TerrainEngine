#pragma once

#include "frame.h"

namespace Engine {

    class FrameDefault {
    private:

    public:

        unsigned int shaderProgramId;
        glm::u16vec2 size;
        unsigned int planeVAO;

        // uniforms
        unsigned int textureId;

        FrameDefault() {}
        FrameDefault(glm::u16vec2 size, unsigned int shaderProgramId, unsigned int textureId);
        ~FrameDefault() {}
        void draw();

    };
}