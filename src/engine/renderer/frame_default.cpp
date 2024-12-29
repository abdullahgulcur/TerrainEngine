#include "pch.h"
#include "frame_default.h"
#include "gl_context.h"

namespace Engine {

    FrameDefault::FrameDefault(glm::u16vec2 size, unsigned int shaderProgramId, unsigned int textureId) {
        this->size = size;
        this->shaderProgramId = shaderProgramId;
        this->textureId = textureId;
        planeVAO = GLBuffer::createQuadVAO();
    }


    void FrameDefault::draw() {

        GLCommand::refreshFrame(size, 0);
        GLShader::useProgram(shaderProgramId);
        GLTexture::useTexture(0, textureId);
        GLCommand::drawQuad(planeVAO);
    }

}
