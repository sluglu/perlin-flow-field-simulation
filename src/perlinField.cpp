#include "perlinField.h"
#include <FastNoiseLite/FastNoiseLite.h>
#include <iostream>

void perlinField::fieldUpdate(){
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    int const l = col;
    float a = l/(PI*2);
    float c = a + 40;
    for (int u = 0; u < l; u++){
        for (int i = 0; i < l; i++){
            float X = (c + a*cos(i*(2*PI/l)))*cos(u*(2*PI/l))*scale;
            float Y = (c + a*cos(i*(2*PI/l)))*sin(u*(2*PI/l))*scale;
            float Z = a*sin(i*(2*PI/l));
            float n = noise.GetNoise(X + offset, Y + offset, Z + offset);
            float b = cos(PI*2*n);
            float o = sin(PI*2*n);
            values[i + u*l] = vec2(b, o);
        }
    }
}

void perlinField::updateVector() {

    while (values.size() > (col + 1) * (col+1)) {
        values.pop_back();
    }
    while (values.size() < (col + 1) * (col + 1)) {
        values.insert(values.end(), vec2(0, 0));
    }
}

