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
        shaders[ShaderType::TERRAIN] = shaderProgramId;
    }

    void Shader::loadTerrainRVT() {

        std::string vertexShaderPath = "../../../resource/shader/terrain_rvt.vert";
        std::string fragmentShaderPath = "../../../resource/shader/terrain_rvt.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("grass", shaderProgramId, 0);
        shaders[ShaderType::TERRAIN_RVT] = shaderProgramId;
    }

}