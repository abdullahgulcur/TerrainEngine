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
        this->textureIdList[2] = GLTexture::generateTerrainPaletteTexture2D(texMacroVariation.width, texMacroVariation.height, texMacroVariation.data);
        texMacroVariation.clean();

        //Texture2D channel_0_a("../../../resource/texture/terrain/grass_lawn_a.png");
        //this->textureIdList[3] = GLTexture::generateTerrainPaletteTexture2D(channel_0_a.width, channel_0_a.height, channel_0_a.data);
        //channel_0_a.clean();

        //Texture2D channel_0_n("../../../resource/texture/terrain/grass_lawn_n.png");
        //this->textureIdList[4] = GLTexture::generateTerrainPaletteTexture2D(channel_0_n.width, channel_0_n.height, channel_0_n.data);
        //channel_0_n.clean();

        //Texture2D channel_1_a("../../../resource/texture/terrain/rock_cliff_a.jpg");
        //this->textureIdList[5] = GLTexture::generateTerrainPaletteTexture2D(channel_1_a.width, channel_1_a.height, channel_1_a.data);
        //channel_1_a.clean();
     
        //Texture2D channel_1_n("../../../resource/texture/terrain/rock_cliff_n.jpg");
        //this->textureIdList[6] = GLTexture::generateTerrainPaletteTexture2D(channel_1_n.width, channel_1_n.height, channel_1_n.data);
        //channel_1_n.clean();

        this->textureIdList[3] = GLTexture::generateCompressedTerrainPaletteTexture2D("../../../resource/texture/terrain_new/grass_lawn_a.dds");
        this->textureIdList[4] = GLTexture::generateCompressedTerrainPaletteTexture2D("../../../resource/texture/terrain_new/grass_lawn_n.dds");
        this->textureIdList[5] = GLTexture::generateCompressedTerrainPaletteTexture2D("../../../resource/texture/terrain_new/rock_cliff_a.dds");
        this->textureIdList[6] = GLTexture::generateCompressedTerrainPaletteTexture2D("../../../resource/texture/terrain_new/rock_cliff_n.dds");

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
