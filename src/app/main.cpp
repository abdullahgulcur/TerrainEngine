#include "pch.h"
#include "core.h"

int main() {

    Engine::Core* core = new Engine::Core;
    core->initialize();

    while (true) {

        core->run();
    }

    return 0;
}