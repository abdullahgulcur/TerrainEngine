#include "pch.h"
#include "frame.h"
#include "gl_context.h"

namespace Engine {

    Frame::Frame() {}

    Frame::~Frame(){}

    void Frame::setViewport(glm::u16vec2 pos, glm::u16vec2 size) {
        this->viewportPos = pos;
        this->viewportSize = size;
    }

    unsigned int Frame::getTextureId() {
        return textureId;
    }

    FramePhsyicalPages::FramePhsyicalPages() {}

    FramePhsyicalPages::FramePhsyicalPages(glm::u16vec2 size, unsigned int grassTextureId, unsigned int shaderProgramId){
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

    FramePageTable::FramePageTable() {}

    FramePageTable::FramePageTable(glm::u16vec2 size, unsigned int shaderProgramId) {
        this->size = size;
        this->shaderProgramId = shaderProgramId;
        textureId = GLTexture::createPhysicalPagesFrameBufferTexture(size);
        FBO = GLBuffer::createFBO(textureId);
        planeVAO = GLBuffer::createQuadVAO();
    }

    FramePageTable::~FramePageTable() {}

    void FramePageTable::setUniforms(glm::u8vec4 color) {
        this->color = color;
    }

    void FramePageTable::draw() {

        GLCommand::setScreen(viewportPos, viewportSize, FBO);
        GLShader::useProgram(shaderProgramId);

        unsigned int c = (color.r << 24) + (color.g << 16) + (color.b << 8) + color.a;

        GLUniform::setUInt1(shaderProgramId, "color", c);
        GLCommand::drawQuad(planeVAO);
    }
}
