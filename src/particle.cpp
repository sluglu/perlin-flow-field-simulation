#include <particle.h>

void particle::update(){
    oldPosi = position;
    speed += acceleration;
    position += speed;
    acceleration = vec2(0,0);
    edges();
    if (length(speed) > maxSpeed) {
	    speed = normalize(speed) * maxSpeed;
    }
}

void particle::applyForce(vec2 force){
    acceleration = acceleration + force;
}


void particle::edges(){
    if (position.y > 1){ position.y = -1;oldPosi = position;}
    if (position.y < -1){ position.y = 1;oldPosi = position;}
    if (position.x > 1){ position.x = -1;oldPosi = position;}
    if (position.x < -1){ position.x = 1;oldPosi = position;}
}
