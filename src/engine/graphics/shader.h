#pragma once

namespace Engine {

    enum class ShaderType {
        //TERRAIN = 0,
        TERRAIN_RVT = 1,
        PBR = 2,
        TERRAIN_PAGE_TABLE = 3,
        FRAME_DEFAULT = 4,
        TERRAIN_GEOMETRY = 5,
        TERRAIN_RVT_COARSE = 6,
        WATER = 7,
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
        //void loadTerrain();
        void loadTerrainGeometry();
        void loadTerrainRVT();
        void loadTerrainRVTCoarse();
        void loadTerrainPageTable();
        void loadWater();
        void loadFrameDefaultShader();
    };
}