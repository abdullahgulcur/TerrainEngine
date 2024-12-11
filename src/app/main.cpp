#include "pch.h"
#include "core.h"
#include "cameractrl.h"

int main() {

    Engine::Core* core = Engine::Core::getInstance();
    Game::CameraController* cameraCtrl = new Game::CameraController;

    core->init();
    cameraCtrl->start();

    core->terrain->init("../../../resource/texture/heightmap.png", static_cast<unsigned short>(Engine::TerrainClipmapSize::BLOCK_SIZE_32), 4);


    while (!core->getGlfwContext()->shouldClose()) {

        core->update();
        cameraCtrl->update();

    }

    return 0;
}
