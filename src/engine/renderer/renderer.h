#pragma once

#include "scene.h"
#include "frame_scene.h"
#include "frame_default.h"

namespace Engine {

    class Renderer {
    private:

    public:

        FrameScene frameScene;
        FrameDefault frameDefault;

        // post process frames;

        static Renderer* create();
        Renderer() {}
        ~Renderer() {}

        void init();
        void loadFrames();
        void draw(Scene* scene);
    };
}