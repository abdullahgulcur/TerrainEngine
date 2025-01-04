#include "pch.h"
#include "frame_default.h"
#include "gl_context.h"

namespace Engine {

    FrameDefault::FrameDefault(glm::u16vec2 size, unsigned int shaderProgramId, unsigned int textureId, unsigned int depthTextureId) {
        this->size = size;
        this->shaderProgramId = shaderProgramId;
        this->textureId = textureId;
        this->depthTextureId = depthTextureId;
        planeVAO = GLBuffer::createQuadVAO();
    }


    void FrameDefault::draw() {

        GLCommand::refreshFrame(size, 0);
        GLShader::useProgram(shaderProgramId);
        GLTexture::useTexture(0, textureId);
        GLTexture::useTexture(1, depthTextureId);
        GLCommand::drawQuad(planeVAO);
    }

}
