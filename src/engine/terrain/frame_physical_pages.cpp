#include "pch.h"
#include "frame_physical_pages.h"
#include "gl_context.h"
#include "image_gen.h"
#include "texture2d.h"
#include "core.h"

namespace Engine {

    FramePhsyicalPages::FramePhsyicalPages(unsigned short pageCounts, unsigned short pageSize, unsigned int heightmapTextureId) {
        //this->blockSize = 1;
        this->pageSize = pageSize;
        this->pageCounts = pageCounts;
        this->textureIdList[0] = heightmapTextureId;
        this->shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_RVT];
        this->shaderProgramId1 = Core::getShader()->shaders[ShaderType::TERRAIN_RVT_COARSE];
        //this->size = glm::u16vec2(pageCounts.x, pageCounts.y) * pageSize;
        UINT8 mipLevels = 10;
        this->textureId = GLTexture::createPhysicalPagesFrameBufferTextureArray(pageSize, mipLevels, this->pageCounts);
        /*this->texture_1_Id = GLTexture::createPhysicalPagesFrameBufferTexture(size);
        this->texture_2_Id = GLTexture::createPhysicalPagesFrameBufferTexture(size);*/
        this->FBO = GLBuffer::createTerrainRVT_FBO(textureId, this->pageCounts);//, texture_1_Id, texture_2_Id
        this->planeVAO = GLBuffer::createQuadVAO();

        ImageGenerator imageGenerator;
        this->textureIdList[1] = imageGenerator.generatePerlinNoiseTexture(glm::u16vec2(2048, 2048), 50.f);

        Texture2D texMacroVariation("../../../resource/texture/macrovariation.png");
        this->textureIdList[2] = texMacroVariation.generateGLTexture();
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
        this->textureIdList[7] = GLTexture::generateCompressedTerrainPaletteTexture2D("../../../resource/texture/terrain_new/bark_soil_a.dds");
        this->textureIdList[8] = GLTexture::generateCompressedTerrainPaletteTexture2D("../../../resource/texture/terrain_new/bark_soil_n.dds");

    }

    void FramePhsyicalPages::setUniforms(unsigned int level, glm::u16vec2 blockPosition) {
        this->level = level;
        this->blockPosition = blockPosition;
    }

    void FramePhsyicalPages::draw(unsigned short tileIndex, UINT8 mipmapLevel) {


        GLCommand::setScreen(glm::u16vec2(0), glm::u16vec2(pageSize), FBO);
        GLBuffer::frameBufferTextureLayer(textureId, tileIndex);

        if (mipmapLevel < 103) { /////

            GLShader::useProgram(shaderProgramId);

            for (int i = 0; i < 9; i++)
                GLTexture::useTexture(i, textureIdList[i]);

            GLUniform::setUInt1(shaderProgramId, "level", level);
            GLUniform::setUInt2(shaderProgramId, "blockPosition", blockPosition);
            //GLUniform::setUInt1(shaderProgramId, "blockSize", blockSize);
            //GLUniform::setUInt1(shaderProgramId, "pageSize", pageSize);
            //GLUniform::setUInt2(shaderProgramId, "pagePosition", pagePosition);
            //GLUniform::setUInt1(shaderProgramId, "pageCounts", pageCounts);
            GLCommand::drawQuad(planeVAO);
        }
        else {

            GLShader::useProgram(shaderProgramId1);

            GLCommand::drawQuad(planeVAO);
        }
    }

}
