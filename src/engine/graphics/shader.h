#pragma once

namespace Engine {

    enum class ShaderType {
        TERRAIN = 0,
        TERRAIN_RVT = 1,
        PBR = 2
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
    };
}