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
    };

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

    class FramePageTable : public Frame {
    private:
        
        glm::u8vec4 color;

    public:

        FramePageTable();
        FramePageTable(glm::u16vec2 size, unsigned int shaderProgramId);
        ~FramePageTable();
        void setUniforms(glm::u8vec4 color);
        void draw();
    };
}