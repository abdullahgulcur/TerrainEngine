#include "pch.h"
#include "frame_physical_pages.h"
#include "gl_context.h"
#include "image_gen.h"
#include "texture2d.h"
#include "core.h"
//#include "stb_image.h"

namespace Engine {

    FramePhsyicalPages::FramePhsyicalPages(unsigned short pageCounts, unsigned short pageSize, unsigned int heightmapTextureId, unsigned int shadowmapTextureId) {
        this->pageSize = pageSize;
        this->pageCounts = pageCounts;
        this->textureIdList[0] = heightmapTextureId;
        this->shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_RVT];
        this->shaderProgramId1 = Core::getShader()->shaders[ShaderType::TERRAIN_RVT_COARSE];
        UINT8 mipLevels = 10;
        this->textureId = GLTexture::createPhysicalPagesFrameBufferTextureArray(pageSize, mipLevels, this->pageCounts);
        this->FBO = GLBuffer::createTerrainRVT_FBO(textureId, this->pageCounts);//, texture_1_Id, texture_2_Id
        this->planeVAO = GLBuffer::createQuadVAO();

        ImageGenerator imageGenerator;
        this->textureIdList[1] = imageGenerator.generatePerlinNoiseTexture(glm::u16vec2(2048, 2048), 5.f);

        //Texture2D texMacroVariation("../../../resource/texture/macrovariation.png");
        //this->textureIdList[2] = texMacroVariation.generateGLTexture();
        //texMacroVariation.clean();

        this->textureIdList[2] = GLTexture::generateMacroVariationTexture("../../../resource/texture/macrovariation.png");

        this->textureIdList[3] = shadowmapTextureId;

        std::vector<std::string> texturePathList;
        texturePathList.push_back("../../../resource/texture/terrain_new/results/grass_dried_a.dds");
        texturePathList.push_back("../../../resource/texture/terrain_new/results/bark_soil_a.dds");//wild_grass_a
        texturePathList.push_back("../../../resource/texture/terrain_new/results/rock_cliff_a.dds");
        texturePathList.push_back("../../../resource/texture/terrain_new/results/grass_dried_n.dds");
        texturePathList.push_back("../../../resource/texture/terrain_new/results/bark_soil_n.dds");//wild_grass_n
        texturePathList.push_back("../../../resource/texture/terrain_new/results/rock_cliff_n.dds");
        texturePathList.push_back("../../../resource/texture/terrain_new/results/rock_cliff_a_.dds");
        texturePathList.push_back("../../../resource/texture/terrain_new/results/rock_cliff_n_.dds");
        texturePathList.push_back("../../../resource/texture/terrain_new/sand_beach_a.dds");
        texturePathList.push_back("../../../resource/texture/terrain_new/sand_beach_n.dds");

        texturePaletteTextureArrayId = GLTexture::generateCompressedTerrainPaletteTextureArray(texturePathList, 1024);
    }

    void FramePhsyicalPages::setUniforms(unsigned int level, glm::u16vec2 blockPosition) {
        this->level = level;
        this->blockPosition = blockPosition;
    }

    void FramePhsyicalPages::draw(unsigned short tileIndex, UINT8 mipmapLevel) {

        GLCommand::setScreen(glm::u16vec2(0), glm::u16vec2(pageSize), FBO);
        GLBuffer::frameBufferTextureLayer(textureId, tileIndex);

        if (mipmapLevel <= 90)
            GLShader::useProgram(shaderProgramId);
        else
            GLShader::useProgram(shaderProgramId1);

        for (int i = 0; i < 3; i++)
            GLTexture::useTexture(i, textureIdList[i]);
        GLTexture::useTextureArray(3, texturePaletteTextureArrayId);
        GLTexture::useTexture(4, textureIdList[3]);
        GLUniform::setUInt1(shaderProgramId, "level", level);
        GLUniform::setUInt2(shaderProgramId, "blockPosition", blockPosition);
        GLCommand::drawQuad(planeVAO);
    }

}
