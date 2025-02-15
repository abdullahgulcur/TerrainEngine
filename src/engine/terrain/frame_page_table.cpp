#include "pch.h"
#include "frame_page_table.h"
#include "gl_context.h"
#include "core.h"

namespace Engine {

    FramePageTable::FramePageTable(glm::u8vec2 mipCounts) {

        this->mipCounts = mipCounts;
        this->size = mipCounts * glm::u8vec2(5);
        this->shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_PAGE_TABLE];
        textureId = GLTexture::createPageTableFrameBufferTexture(size);
        FBO = GLBuffer::createFBO(textureId);
        planeVAO = GLBuffer::createQuadVAO();
    }

    void FramePageTable::setUniforms(unsigned int color, unsigned int level) {

        this->color = color;
    }

    void FramePageTable::draw() {

        GLCommand::setScreen(viewportPos, viewportSize, FBO);
        GLShader::useProgram(shaderProgramId);

        GLUniform::setUInt1(shaderProgramId, "color", color);
        GLCommand::drawQuad(planeVAO);
    }
}
