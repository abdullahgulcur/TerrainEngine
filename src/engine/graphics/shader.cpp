#include "pch.h"
#include "shader.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "config.h"

namespace Engine {

    Shader* Shader::create() {

        return new Shader;
    }

    Shader::Shader() {}

    Shader::~Shader() {}

    void Shader::init() {

        Shader::loadPBR();
        Shader::loadEnvironmentCubemapBackground();
        Shader::loadTerrainRVT();
        Shader::loadTerrainRVTCoarse();
        Shader::loadTerrainPageTable();
        Shader::loadFrameDefaultShader();
        Shader::loadTerrainGeometry();
        Shader::loadShadowmap();
    }

    void Shader::loadPBR() {

    }

    void Shader::loadEnvironmentCubemapBackground() {

    }

    void Shader::loadTerrainGeometry() {

        std::string resource_dir(RESOURCES);

        std::string vertexShaderPath = resource_dir + "/shader/terrain/terrain_geometry.vert";
        std::string fragmentShaderPath = resource_dir + "/shader/terrain/terrain_geometry.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("heightmap", shaderProgramId, 0);
        GLTexture::setTextureUniformLocation("pageTable", shaderProgramId, 1);
        GLTexture::setTextureUniformLocation("physicalPages", shaderProgramId, 2);
        GLTexture::setTextureUniformLocation("shadowMap", shaderProgramId, 3);
        shaders[ShaderType::TERRAIN_GEOMETRY] = shaderProgramId;
    }

    void Shader::loadTerrainRVT() {

        std::string resource_dir(RESOURCES);

        std::string vertexShaderPath = resource_dir + "/shader/terrain/terrain_rvt.vert";
        std::string fragmentShaderPath = resource_dir + "/shader/terrain/terrain_rvt.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("heightmap", shaderProgramId, 0);
        GLTexture::setTextureUniformLocation("tex1", shaderProgramId, 1);
        GLTexture::setTextureUniformLocation("tex2", shaderProgramId, 2);
        GLTexture::setTextureUniformLocation("palette", shaderProgramId, 3);
        shaders[ShaderType::TERRAIN_RVT] = shaderProgramId;
    }

    void Shader::loadTerrainRVTCoarse() {

        std::string resource_dir(RESOURCES);

        std::string vertexShaderPath = resource_dir + "/shader/terrain/terrain_rvt_coarse.vert";
        std::string fragmentShaderPath = resource_dir + "/shader/terrain/terrain_rvt_coarse.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("heightmap", shaderProgramId, 0);
        GLTexture::setTextureUniformLocation("tex1", shaderProgramId, 1);
        GLTexture::setTextureUniformLocation("tex2", shaderProgramId, 2);
        GLTexture::setTextureUniformLocation("palette", shaderProgramId, 3);
        GLTexture::setTextureUniformLocation("shadowmap", shaderProgramId, 4);
        shaders[ShaderType::TERRAIN_RVT_COARSE] = shaderProgramId;
    }

    void Shader::loadTerrainPageTable() {

        std::string resource_dir(RESOURCES);

        std::string vertexShaderPath = resource_dir + "/shader/terrain/terrain_page_table.vert";
        std::string fragmentShaderPath = resource_dir + "/shader/terrain/terrain_page_table.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        shaders[ShaderType::TERRAIN_PAGE_TABLE] = shaderProgramId;
    }

    void Shader::loadWater() {

        std::string resource_dir(RESOURCES);

        std::string vertexShaderPath = resource_dir + "/shader/water/water.vert";
        std::string fragmentShaderPath = resource_dir + "/shader/water/water.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        shaders[ShaderType::WATER] = shaderProgramId;
    }

    void Shader::loadFrameDefaultShader() {

        std::string resource_dir(RESOURCES);

        std::string vertexShaderPath = resource_dir + "/shader/frame_default.vert";
        std::string fragmentShaderPath = resource_dir + "/shader/frame_default.frag";
        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath);
        GLTexture::setTextureUniformLocation("frameTexture", shaderProgramId, 0);
        GLTexture::setTextureUniformLocation("frameDepthTexture", shaderProgramId, 1);
        GLTexture::setTextureUniformLocation("depthTextureArray", shaderProgramId, 2);
        shaders[ShaderType::FRAME_DEFAULT] = shaderProgramId;
    }

    void Shader::loadShadowmap() {

        std::string resource_dir(RESOURCES);
        std::string vertexShaderPath = resource_dir + "/shader/shadow/shadow_mapping_depth.vert";
        std::string fragmentShaderPath = resource_dir + "/shader/shadow/shadow_mapping_depth.frag";
        std::string geometryShaderPath = resource_dir + "/shader/shadow/shadow_mapping_depth.geom";

        unsigned int shaderProgramId = GLShader::loadShader(vertexShaderPath, fragmentShaderPath, geometryShaderPath);
        GLTexture::setTextureUniformLocation("heightmap", shaderProgramId, 0);

        shaders[ShaderType::SHADOW_MAPPING] = shaderProgramId;

    }

}