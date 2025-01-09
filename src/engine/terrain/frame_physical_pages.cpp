#include "pch.h"
#include "frame_physical_pages.h"
#include "gl_context.h"
#include "image_gen.h"
#include "texture2d.h"
#include "core.h"

namespace Engine {

    FramePhsyicalPages::FramePhsyicalPages() {}

    FramePhsyicalPages::FramePhsyicalPages(glm::u16vec2 size, unsigned int heightmapTextureId, unsigned int blockSize) {
        this->size = size;
        this->blockSize = blockSize;
        this->textureIdList[0] = heightmapTextureId;
        this->shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_RVT];
        this->texture_0_Id = GLTexture::createPhysicalPagesFrameBufferTexture(size);
        /*this->texture_1_Id = GLTexture::createPhysicalPagesFrameBufferTexture(size);
        this->texture_2_Id = GLTexture::createPhysicalPagesFrameBufferTexture(size);*/
        this->FBO = GLBuffer::createTerrainRVT_FBO(texture_0_Id);//, texture_1_Id, texture_2_Id
        this->planeVAO = GLBuffer::createQuadVAO();

        ImageGenerator imageGenerator;
        this->textureIdList[1] = imageGenerator.generatePerlinNoiseTexture(glm::u16vec2(1024, 1024), 50.f);

        Texture2D texMacroVariation("../../../resource/texture/macrovariation.png");
        this->textureIdList[2] = texMacroVariation.generateGLTexture();
        texMacroVariation.clean();

        Texture2D channel_0_a("../../../resource/texture/grasslawn_a.png");
        this->textureIdList[3] = channel_0_a.generateGLTexture();
        channel_0_a.clean();

        Texture2D channel_0_n("../../../resource/texture/grasslawn_n.png");
        this->textureIdList[4] = channel_0_n.generateGLTexture();
        channel_0_n.clean();

        Texture2D channel_1_a("../../../resource/texture/rock_cliff_a.jpg");
        this->textureIdList[5] = channel_1_a.generateGLTexture();
        channel_1_a.clean();
     
        Texture2D channel_1_n("../../../resource/texture/rock_cliff_n.jpg");
        this->textureIdList[6] = channel_1_n.generateGLTexture();
        channel_1_n.clean();

        /*Texture2D channel_0_d("../../../resource/texture/grasslawn_d.jpg");
        this->textureIdList[7] = channel_0_d.generateGLTexture();
        channel_0_d.clean();

        Texture2D channel_1_d("../../../resource/texture/rock_cliff_d.jpg");
        this->textureIdList[8] = channel_1_d.generateGLTexture();
        channel_1_d.clean();*/
    }

    FramePhsyicalPages::~FramePhsyicalPages() {}

    void FramePhsyicalPages::setUniforms(unsigned int level, glm::u16vec2 blockPosition, glm::u8vec2 pagePosition, glm::u8vec2 physicalTextureSize) {
        this->level = level;
        this->blockPosition = blockPosition;
        this->pagePosition = pagePosition;
        this->physicalTextureSize = physicalTextureSize;
    }

    void FramePhsyicalPages::draw() {

        GLCommand::setScreen(viewportPos, viewportSize, FBO);
        GLShader::useProgram(shaderProgramId);

        for (int i = 0; i < 7; i++) 
            GLTexture::useTexture(i, textureIdList[i]);

        GLUniform::setUInt1(shaderProgramId, "level", level);
        GLUniform::setUInt2(shaderProgramId, "blockPosition", blockPosition);
        GLUniform::setUInt1(shaderProgramId, "blockSize", blockSize);
        GLUniform::setUInt2(shaderProgramId, "pagePosition", pagePosition);
        GLUniform::setUInt2(shaderProgramId, "physicalTextureSize", physicalTextureSize);
        GLCommand::drawQuad(planeVAO);
    }

    void FramePhsyicalPages::setViewport(glm::u16vec2 pos, glm::u16vec2 size) {
        this->viewportPos = pos;
        this->viewportSize = size;
    }
}
