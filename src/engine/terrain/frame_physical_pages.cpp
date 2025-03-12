#include "pch.h"
#include "frame_physical_pages.h"
#include "gl_context.h"
#include "image_gen.h"
#include "core.h"
#include "config.h"
//#include "stb_image.h"

namespace Engine {

    FramePhsyicalPages::FramePhsyicalPages(unsigned short pageCounts, unsigned short pageSize, unsigned int heightmapTextureId, unsigned int shadowmapTextureId) {
        this->pageSize = pageSize;
        this->pageCounts = pageCounts;
        this->textureIdList[0] = heightmapTextureId;
        this->shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_RVT];
        this->shaderProgramId1 = Core::getShader()->shaders[ShaderType::TERRAIN_RVT_COARSE];
        UINT8 mipLevels = 9;
        this->textureId = GLTexture::createPhysicalPagesFrameBufferTextureArray(pageSize, mipLevels, this->pageCounts);
        this->FBO = GLBuffer::createTerrainRVT_FBO(textureId, this->pageCounts);
        this->planeVAO = GLBuffer::createQuadVAO();

        ImageGenerator imageGenerator;
        this->textureIdList[1] = imageGenerator.generatePerlinNoiseTexture(glm::u16vec2(1024, 1024), 25.f);

        std::string resource_dir(RESOURCES);

        this->textureIdList[2] = GLTexture::generateMacroVariationTexture(resource_dir + "/texture/terrain/macrovariation.png");
        this->textureIdList[3] = shadowmapTextureId;

        std::vector<std::string> texturePathList;
        texturePathList.push_back(resource_dir + "/texture/terrain/soil_rock_a.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/grass_uncut_a.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/grass_wild_a.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/rock_cliff_a.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/sand_debris_a.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/soil_rock_n.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/grass_uncut_n.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/grass_wild_n.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/rock_cliff_n.dds");
        texturePathList.push_back(resource_dir + "/texture/terrain/sand_debris_n.dds");

        texturePaletteTextureArrayId = GLTexture::generateCompressedTerrainPaletteTextureArray(texturePathList, 512);
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
        //GLTexture::useTexture(4, textureIdList[3]);
        GLUniform::setUInt1(shaderProgramId, "level", level);
        GLUniform::setUInt2(shaderProgramId, "blockPosition", blockPosition);
        GLCommand::drawQuad(planeVAO);
    }

}
