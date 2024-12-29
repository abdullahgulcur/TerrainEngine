#pragma once

#include "glm.hpp"

namespace Engine {

    class Frame {
    protected:

        unsigned int shaderProgramId;
        unsigned int planeVAO;
        unsigned int FBO;
        unsigned int textureId;
        glm::u16vec2 size;

        glm::u16vec2 viewportPos;
        glm::u16vec2 viewportSize;

        Frame();
        ~Frame();

    public:
        void setViewport(glm::u16vec2 pos, glm::u16vec2 size);
        virtual void draw() = 0;
        unsigned int getTextureId();
        glm::u16vec2 getSize();
    };

}