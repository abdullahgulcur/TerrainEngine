#include "pch.h"
#include "frame_page_table.h"
#include "gl_context.h"

namespace Engine {

    FramePageTable::FramePageTable() {}

    FramePageTable::FramePageTable(glm::u16vec2 size, unsigned int shaderProgramId) {
        this->size = size;
        this->shaderProgramId = shaderProgramId;
        textureId = GLTexture::createPageTableFrameBufferTexture(size);
        FBO = GLBuffer::createFBO(textureId);
        planeVAO = GLBuffer::createQuadVAO();
    }

    FramePageTable::~FramePageTable() {}

    void FramePageTable::setUniforms(unsigned int color) {
        this->color = color;
    }

    void FramePageTable::draw() {

        GLCommand::setScreen(viewportPos, viewportSize, FBO);
        GLShader::useProgram(shaderProgramId);

        GLUniform::setUInt1(shaderProgramId, "color", color);
        GLCommand::drawQuad(planeVAO);
    }
}
