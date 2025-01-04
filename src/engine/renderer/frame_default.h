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
        unsigned int depthTextureId;

        FrameDefault() {}
        FrameDefault(glm::u16vec2 size, unsigned int shaderProgramId, unsigned int textureId, unsigned int depthTextureId);
        ~FrameDefault() {}
        void draw();

    };
}