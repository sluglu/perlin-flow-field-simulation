#include <glm/glm.hpp>
#include <perlinField.h>

using namespace glm;

class particle{
    public:
        float maxSpeed = 0.003f;
        vec4 color = vec4(1,1,1, 1);
        vec2 position = vec2(cos(rand()),cos(rand()));
        vec2 oldPosi = position;
        vec2 speed = vec2(0,0);
        vec2 acceleration = vec2(0,0);
        void update();
        void applyForce(vec2);
        void edges();

        particle() = default;
};