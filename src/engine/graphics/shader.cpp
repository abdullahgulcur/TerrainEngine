#include "pch.h"
#include "shader.h"
#include "gl_shader.h"
#include "gl_texture.h"

namespace Engine {

    Shader* Shader::create() {

        return new Shader;
    }

    Shader::Shader() {}

    Shader::~Shader() {}

    void Shader::init() {

        Shader::loadPBR();
        Shader::loadEnvironmentCubemapBackground();
        Shader::loadTerrain();
        Shader::loadTerrainRVT();
        Shader::loadTerrainPageTable();
        Shader::loadFrameDefaultShader();
    }

    void Shader::loadPBR() {

    }

    void Shader::loadEnvironmentCubemapBackground() {

    }

    void Shader::loadTerrain() {

        std::string vertexShaderPath = "../../../resource/shader/terrain.vert";
        std::string fragmentShaderPath = "../../../resource/shader/terrain.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("pageTable", shaderProgramId, 0);
        GLTexture::setTextureUniformLocation("physicalHeightmap", shaderProgramId, 1);
        GLTexture::setTextureUniformLocation("physicalPages", shaderProgramId, 2);
        /*GLTexture::setTextureUniformLocation("physicalPagesNormal", shaderProgramId, 3);
        GLTexture::setTextureUniformLocation("physicalPagesDisplacement", shaderProgramId, 4); */
        shaders[ShaderType::TERRAIN] = shaderProgramId;
    }

    void Shader::loadTerrainRVT() {

        std::string vertexShaderPath = "../../../resource/shader/terrain_rvt.vert";
        std::string fragmentShaderPath = "../../../resource/shader/terrain_rvt.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("tex0", shaderProgramId, 0);
        GLTexture::setTextureUniformLocation("tex1", shaderProgramId, 1);
        GLTexture::setTextureUniformLocation("tex2", shaderProgramId, 2);
        GLTexture::setTextureUniformLocation("tex3", shaderProgramId, 3);
        GLTexture::setTextureUniformLocation("tex4", shaderProgramId, 4);
        GLTexture::setTextureUniformLocation("tex5", shaderProgramId, 5);
        GLTexture::setTextureUniformLocation("tex6", shaderProgramId, 6);
        /*GLTexture::setTextureUniformLocation("tex7", shaderProgramId, 7);
        GLTexture::setTextureUniformLocation("tex8", shaderProgramId, 8);*/
        shaders[ShaderType::TERRAIN_RVT] = shaderProgramId;
    }

    void Shader::loadTerrainPageTable() {

        std::string vertexShaderPath = "../../../resource/shader/terrain_page_table.vert";
        std::string fragmentShaderPath = "../../../resource/shader/terrain_page_table.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        shaders[ShaderType::TERRAIN_PAGE_TABLE] = shaderProgramId;
    }

    void Shader::loadFrameDefaultShader() {

        std::string vertexShaderPath = "../../../resource/shader/frame_default.vert";
        std::string fragmentShaderPath = "../../../resource/shader/frame_default.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("frameTexture", shaderProgramId, 0);
        GLTexture::setTextureUniformLocation("frameDepthTexture", shaderProgramId, 1);
        shaders[ShaderType::FRAME_DEFAULT] = shaderProgramId;
    }

}