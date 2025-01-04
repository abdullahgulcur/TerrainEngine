#pragma once

#include "frame.h"

namespace Engine {

    class FrameScene {
    private:

    public:

        unsigned int FBO;
        unsigned int RBO;
        unsigned int textureId;
        unsigned int depthTextureId;
        glm::u16vec2 size;

        FrameScene();
        FrameScene(glm::u16vec2 size);
        ~FrameScene();
        void setScreen();
        
    };
}