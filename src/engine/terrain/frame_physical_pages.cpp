#include "pch.h"
#include "frame_physical_pages.h"
#include "gl_context.h"

namespace Engine {

    FramePhsyicalPages::FramePhsyicalPages() {}

    FramePhsyicalPages::FramePhsyicalPages(glm::u16vec2 size, unsigned int grassTextureId, unsigned int shaderProgramId) {
        this->size = size;
        this->grassTextureId = grassTextureId;
        this->shaderProgramId = shaderProgramId;
        textureId = GLTexture::createPhysicalPagesFrameBufferTexture(size);
        FBO = GLBuffer::createFBO(textureId);
        planeVAO = GLBuffer::createQuadVAO();
    }

    FramePhsyicalPages::~FramePhsyicalPages() {}

    void FramePhsyicalPages::setUniforms(unsigned int level, glm::vec2 position) {
        this->level = level;
        this->position = position;
    }

    void FramePhsyicalPages::draw() {

        GLCommand::setScreen(viewportPos, viewportSize, FBO);
        GLShader::useProgram(shaderProgramId);
        GLTexture::useTexture(0, grassTextureId);
        GLUniform::setInt1(shaderProgramId, "level", level);
        GLUniform::setFloat2(shaderProgramId, "pos", position);
        GLCommand::drawQuad(planeVAO);
    }

}
