#include <GLContext.h>
#include <iostream>
#include <particle.h>
#include <random>
#include <perlinField.h>
#include <thread>

bool showField = false;
bool showPart = true;
bool drawMode = true;
float flowForce = 0.001;
vec4 particlesColor = vec4(1,1,1, 1);
bool incroffset = false;
float offsetincr = 0;

bool paused = false;
int particlesNum = 1000;
vector<particle> particles;
perlinField field;

using namespace GLContext;

void followField(int index){
    int x = particles[index].position.x * (field.col/2) + field.col/2;
    int y = particles[index].position.y * (field.col/2) + field.col/2;
    vec2 force = vec2(field.values[x + y*field.col].x*flowForce,field.values[x + y*field.col].y*flowForce);
    particles[index].applyForce(force);
}

void update(int a, int b) {
    if (incroffset) { field.offset += offsetincr; }
    for (int i = a; i < b; i++) {
        followField(i);
        particles[i].update();
    }
}

void particleUpdate(){
    int numThread = std::thread::hardware_concurrency();
    int c = particlesNum/numThread;
    std::vector<std::thread> threads(numThread);
    for(int i = 0; i < numThread; i++){
        threads[i] = std::thread(update, i*c, (i+1)*c);
    }
    for(int i = 0; i < numThread; i++){
        threads[i].join();
    }
}

void particleDraw(){
    for (int i = 0; i < particlesNum; i++){
        if (drawMode) {
            drawLine(particles[i].position, particles[i].oldPosi, 1.0f, particlesColor);
        }
        else {
            drawPoint(particles[i].position, 5.0f, particlesColor);
        }
    }
}

void fieldDraw() {
    int col = field.col;
    for (int y = 0; y < col + 1; y++) {
        for (int x = 0; x < col + 1; x++) {
            vec2 pos1 = vec2((x - col / 2.0f) / (col / 2.0f), (y - col / 2.0f) / (col / 2.0f));
            vec2 pos2 = vec2(((x - col / 2.0f) / (col / 2.0f) + field.values[x + y * col].x * 0.01f), (y - col / 2.0f) / (col / 2.0f) + field.values[x + y * col].y * 0.01f);
            drawLine(pos1, pos2, 1.5f, field.color);
        }
    }
}


void resetPos(){
    for (int i=0; i < particlesNum; i++){
        particles[i].position = vec2(cos(rand()),cos(rand()));
        particles[i].oldPosi = particles[i].position;
    }

}

void updateVector() {
    while (particles.size() > particlesNum) {
        particles.pop_back();
    }
    while (particles.size() < particlesNum) {
        particle p;
        particles.insert(particles.end(), p);
    }
}

void start() {
    particles.clear();
    updateVector();
    field.updateVector();
}

void reset() {
    particles.clear();
    updateVector();
}

void ui(){
    ImGui::Begin("Settings"); 

    if(ImGui::InputInt("Particles Number", &particlesNum)) {updateVector();}

    if (ImGui::InputInt("field subdivision", &field.col)) {field.updateVector();}

    ImGui::Checkbox("Show Alpha", &alpha);
    ImGui::Checkbox("Draw lines/points", &drawMode);

    ImGui::Checkbox("Show Particles", &showPart); 
    ImGui::SameLine(); 
    ImGui::Checkbox("Show Field", &showField);

    if (ImGui::Button("Reset Flow Force")){flowForce=0.0001;}
    ImGui::SameLine(); 
    ImGui::SliderFloat("Flow Force", &flowForce, -0.1f, 0.1f);
    
    
    if (ImGui::Button("Reset Offset")){field.offset=0;}
    ImGui::SameLine(); 
    ImGui::SliderFloat("Perlin Field Offset", &field.offset, -100.0f, 100.0f);
    
    
    if (ImGui::Button("Reset Scale")){field.scale=1;}
    ImGui::SameLine(); 
    ImGui::SliderFloat("Perlin Field Scale", &field.scale, -4.0f, 4.0f);
    
    ImGui::Checkbox("increase offsets", &incroffset); 
    ImGui::SameLine(); 
    ImGui::SliderFloat("offset increment", &offsetincr, -0.1f, 0.1f);

    ImGui::ColorEdit4("particle color", &particlesColor[0]);
    ImGui::ColorEdit4("field color", &field.color[0]);
    ImGui::ColorEdit4("background color", &background[0]);
    
    if (ImGui::Button("pause")) { paused = !paused; }
    ImGui::SameLine();
    if (ImGui::Button("reset")) {reset();}
    ImGui::SameLine();
    if (ImGui::Button("clear")) { alphaClear(); }
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void frame(){
    field.fieldUpdate();
    if(showPart) {particleDraw();}
    if(showField){fieldDraw();}
    if(!paused){
        particleUpdate();
    }
}



int main(){
    onDraw = frame;
    onDrawUI = ui;
    initialize = start;
    viewportResH, viewportResW= 2000;
    init(1500,1000);

    return 0;
}
