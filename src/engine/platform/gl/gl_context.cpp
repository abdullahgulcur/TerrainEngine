#include "pch.h"
#include "gl_context.h"

namespace Engine {

    void GLContext::init() {

        glewExperimental = true;
        glewInit();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glDepthFunc(GL_LEQUAL);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }


}