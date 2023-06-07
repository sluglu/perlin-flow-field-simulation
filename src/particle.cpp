#include <particle.h>

void particle::update(){
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

void particle::draw(bool point, int plylimit, bool plydraw){
    glPointSize(1);
    if(point == false){
        glBegin(GL_LINES);
        glColor4f(color.x,color.y,color.z, color.w);
        glVertex2f(position.x, position.y);
        glVertex2f(oldPosi.x,oldPosi.y);
    }
    if(point == true){
        glPointSize(1);
        glBegin(GL_POINTS);
        glColor4f(color.x,color.y,color.z, color.w);
        glVertex2f(position.x, position.y);
    }
    glEnd();
    if(plycount >= plylimit && plydraw == true && point == false){oldPosi = position;plycount = 0;}
    if(point == false && plydraw == false){oldPosi = position;}
    plycount += 1;
}

void particle::edges(){
    if (position.y > 1){ position.y = -1;oldPosi = position;}
    if (position.y < -1){ position.y = 1;oldPosi = position;}
    if (position.x > 1){ position.x = -1;oldPosi = position;}
    if (position.x < -1){ position.x = 1;oldPosi = position;}
}