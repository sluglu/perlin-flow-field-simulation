#pragma once
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

class perlinField{
    public:
        float PI = 3.14159265359;
        int col = 100;
        vec4 color = vec4(1,1,1,1);
        float scale = 1;
        float offset = 0;
        std::vector<vec2> values;
        void fieldUpdate();
        void updateVector();
};