#pragma once
#include <glm/glm.hpp>
#include <GL/freeglut.h>

using namespace glm;

class perlinField{
    public:
        float PI = 3.14159265359;
        int const static col = 100;
        vec4 color = vec4(1,1,1,1);
        float scale = 1;
        float torusRadOff = 40;
        float offset = 0;
        vec2 field[col*col];
        void fieldUpdate();
        void fieldDraw();
};