#include "pch.h"
#include "core.h"
#include "cameractrl.h"

int main() {

    //Engine code
    Engine::Core* core = Engine::Core::getInstance();
    core->init();

    //Game code
    Game::CameraController* cameraCtrl = new Game::CameraController;
    cameraCtrl->start();
    Engine::Terrain* terrain = new Engine::Terrain;
    core->scene->terrain = terrain;
    terrain->init(glm::u16vec2(8192), static_cast<unsigned short>(Engine::TerrainClipmapSize::BLOCK_SIZE_64), 3, 1);

    while (!core->getGlfwContext()->shouldClose()) {

        //Engine code
        core->update();

        //Game code
        cameraCtrl->update();
    }

    return 0;
}
