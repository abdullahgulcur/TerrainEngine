#include "pch.h"
#include "scene.h"
#include "core.h"

namespace Engine {

    Scene* Scene::create() {

        return new Scene;
    }

    void Scene::init() {

    }

    void Scene::update() {
        
        if (terrain)
            terrain->update();
    }

}

