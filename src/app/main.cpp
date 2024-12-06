#include "pch.h"
#include "core.h"

int main() {

    Engine::Core* core = Engine::Core::getInstance();

    core->init();

    while (!core->getGlfwContext()->shouldClose()) {

        core->update();
    }

    return 0;
}
