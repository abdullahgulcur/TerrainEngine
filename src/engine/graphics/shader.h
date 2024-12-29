#pragma once

namespace Engine {

    enum class ShaderType {
        TERRAIN = 0,
        TERRAIN_RVT = 1,
        PBR = 2,
        TERRAIN_PAGE_TABLE = 3,
        FRAME_DEFAULT = 4
    };

    class Shader {
    private:
    public:

        std::map<ShaderType, unsigned int> shaders;

        static Shader* create();
        Shader();
        ~Shader();
        void init();
        void loadPBR();
        void loadEnvironmentCubemapBackground();
        void loadTerrain();
        void loadTerrainRVT();
        void loadTerrainPageTable();
        void loadFrameDefaultShader();
    };
}