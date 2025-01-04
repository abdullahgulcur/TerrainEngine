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
        this->heightmapTextureId = heightmapTextureId;
        this->shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_RVT];
        this->textureId = GLTexture::createPhysicalPagesFrameBufferTexture(size);
        this->FBO = GLBuffer::createFBO(textureId);
        this->planeVAO = GLBuffer::createQuadVAO();

        Texture2D tex0("../../../resource/texture/grasslawn_a.png");
        this->tex_0_TextureId = GLTexture::generateTexture2D(tex0.channels, tex0.width, tex0.height, tex0.data);
        tex0.clean();

        Texture2D tex1("../../../resource/texture/cliffgranite_a.png");
        this->tex_1_TextureId = GLTexture::generateTexture2D(tex1.channels, tex1.width, tex1.height, tex1.data);
        tex1.clean();

        ImageGenerator imageGenerator;
        tex_2_TextureId = imageGenerator.generatePerlinNoiseTexture(glm::u16vec2(512, 512), 50.f);

        Texture2D tex2("../../../resource/texture/grasswild_a.png");
        this->tex_3_TextureId = GLTexture::generateTexture2D(tex2.channels, tex2.width, tex2.height, tex2.data);
        tex2.clean();

        //Texture2D textureData(512, 512, 1);
        //GLTexture::getTextureContent(1, textureData.data, tex_2_TextureId);
        //textureData.writeDataToFile("fuckeriiii.png", 0);
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
        GLTexture::useTexture(0, heightmapTextureId);
        GLTexture::useTexture(1, tex_0_TextureId);
        GLTexture::useTexture(2, tex_1_TextureId);
        GLTexture::useTexture(3, tex_2_TextureId);
        GLTexture::useTexture(4, tex_3_TextureId);
        GLUniform::setUInt1(shaderProgramId, "level", level);
        GLUniform::setUInt2(shaderProgramId, "blockPosition", blockPosition);
        GLUniform::setUInt1(shaderProgramId, "blockSize", blockSize);
        GLUniform::setUInt2(shaderProgramId, "pagePosition", pagePosition);
        GLUniform::setUInt2(shaderProgramId, "physicalTextureSize", physicalTextureSize);
        GLCommand::drawQuad(planeVAO);
    }
}
