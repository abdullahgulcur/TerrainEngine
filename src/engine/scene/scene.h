#pragma once

#include "terrain.h"
#include "frame_scene.h"

namespace Engine {

    class Scene { // TerrainScene is more meaningful.
    private:
    public:

        Terrain* terrain = NULL;

        static Scene* create();
        Scene() {}
        ~Scene() {}
        void init();
        void update();
    };
}