#include "perlinField.h"
#include <FastNoiseLite.h>
#include <iostream>

void perlinField::fieldUpdate(){
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    int const l = col;
    float values[l*l];
    float a = l/(PI*2);
    float c = a + torusRadOff;
    for (int u = 0; u <= l; u++){
        for (int i = 0; i <= l; i++){
            float X = (c + a*cos(i*(2*PI/l)))*cos(u*(2*PI/l))*scale+offset;
            float Y = (c + a*cos(i*(2*PI/l)))*sin(u*(2*PI/l))*scale+offset;
            float Z = a*sin(i*(2*PI/l));
            float n = noise.GetNoise(X, Y, Z);
            float b = cos(PI*2*n);
            float o = sin(PI*2*n);
            field[i + u*l] = vec2(b, o);
        }
    }
}

void perlinField::fieldDraw(){
    for (int y = 0; y < col+1; y++){
        for (int x = 0; x < col+1; x++){
            glPointSize(2);
            glBegin(GL_LINES);
            glColor4f(color.x,color.y,color.z,color.w);
            glVertex2f((x - col/2.0f)/(col/2.0f), (y - col/2.0f)/(col/2.0f));
            glVertex2f(((x - col/2.0f)/(col/2.0f) + field[x + y*col].x*0.01f), (y - col/2.0f)/(col/2.0f) + field[x + y*col].y*0.01f);
            glEnd();
        }
    }
}