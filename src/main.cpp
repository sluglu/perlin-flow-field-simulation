#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <particle.h>
#include <random>
#include <perlinField.h>
#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>
#include <thread>

bool alpha = false;
bool plydraw = false;
int plylimit = 5;
bool showField = false;
bool showPart = true;
bool drawMode = true;
bool applyF = true;
bool applyC = true;
float flowForce = 0.001;
float cForce = 0.001;
vec4 particlesColor = vec4(1,1,1, 1);
bool incroffset = false;
float offsetincr = 0;

bool clear = false;
bool paused = false;
bool simulating = false;
int particlesNum = 100000;
particle particles[1000000];
int const width = 2560;
int const height = 1600;
perlinField field;

int getResolution(){
    int input;
    std::cout << "Enter the number of particles : ";
    std::cin >> input;
    return input;
}

void followField(int index){
    int x = particles[index].position.x * (field.col/2) + field.col/2;
    int y = particles[index].position.y * (field.col/2) + field.col/2;
    vec2 force = vec2(field.field[x + y*field.col].x*flowForce,field.field[x + y*field.col].y*flowForce);
    particles[index].applyForce(force);
}

void centerForce(int index){
    particle p = particles[index];
    float dst = p.position.length();
    float x = (-p.position.x/sqrt(dst)*cForce);
    float y = (-p.position.y/sqrt(dst)*cForce);
    vec2 force = vec2(x,y);
    particles[index].applyForce(force);
}

void update(int a, int b){
        if(incroffset == true){field.offset += offsetincr;}
        for (int i=a; i < b; i++){
            if(applyF){followField(i);}
            if(applyC){centerForce(i);}
            particles[i].update();
        }
    }

void particleUpdate(){
    int numThread = std::thread::hardware_concurrency();
    int c = particlesNum/numThread;
    std::thread threads[numThread];
    for(int i = 0; i < numThread; i++){
        threads[i] = std::thread(update, i*c, (i+1)*c);
    }
    for(int i = 0; i < numThread; i++){
        threads[i].join();
    }
}

void particleDraw(bool mode){
    for (int i=0; i < particlesNum; i++){
        particles[i].draw(mode, plylimit, plydraw);
        particles[i].color = particlesColor;
    }
}

void resetPos(){
    for (int i=0; i < particlesNum; i++){
        particles[i].position = vec2(cos(rand()),cos(rand()));
        particles[i].oldPosi = particles[i].position;
    }

}

void processSpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_F1){if(simulating){paused = !paused;}}
    if (key == GLUT_KEY_F2){simulating = false;resetPos();if(alpha){glEnable(GL_BLEND);}glClear(GL_COLOR_BUFFER_BIT);clear=true;}
}

void GUI(){
    ImGui::Begin("Settings"); 
    ImGui::Text("Particles Number : %i ", particlesNum);
    ImGui::Text("F1 : pause");
    ImGui::SameLine();
    ImGui::Text("F2 : stop simulation");

    ImGui::Checkbox("Show Alpha", &alpha);
    ImGui::Checkbox("Draw lines/points", &drawMode);
    ImGui::SameLine();
    ImGui::Text("(Lines won't show before starting)");

    ImGui::Checkbox("Show Particles", &showPart); 
    ImGui::SameLine(); 
    ImGui::Checkbox("Show Field", &showField);

    if (ImGui::Button("Reset Center Force")){cForce=0.0001;}
    ImGui::SameLine(); 
    ImGui::SliderFloat("Center Force", &cForce, -0.1f, 0.1f);
    ImGui::SameLine(); 
    ImGui::Checkbox("  ", &applyC);

    if (ImGui::Button("Reset Flow Force")){flowForce=0.0001;}
    ImGui::SameLine(); 
    ImGui::SliderFloat("Flow Force", &flowForce, -0.1f, 0.1f);
    ImGui::SameLine(); 
    ImGui::Checkbox(" ", &applyF);
    
    
    if (ImGui::Button("Reset Offset")){field.offset=0;}
    ImGui::SameLine(); 
    ImGui::SliderFloat("Perlin Field Offset", &field.offset, -100.0f, 100.0f);
    
    
    if (ImGui::Button("Reset Scale")){field.scale=1;}
    ImGui::SameLine(); 
    ImGui::SliderFloat("Perlin Field Scale", &field.scale, -1.0f, 1.0f);
    
    ImGui::Checkbox("increase offsets", &incroffset); 
    ImGui::SameLine(); 
    ImGui::SliderFloat("offset increment", &offsetincr, -0.1f, 0.1f);

    ImGui::Checkbox("draw ply", &plydraw); 
    ImGui::SameLine(); 
    ImGui::SliderInt("ply limit", &plylimit, 0, 100);
    
    ImGui::ColorEdit4("clear color", (float*)&particlesColor);
    if (ImGui::Button("Run")){simulating=true;resetPos();paused=false;if(alpha){glEnable(GL_BLEND);}glClear(GL_COLOR_BUFFER_BIT);clear=true;}
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void display(){
    glutSwapBuffers();
    if(simulating){
        if(clear){clear=false;glClear(GL_COLOR_BUFFER_BIT);}
        if(!alpha){glClear(GL_COLOR_BUFFER_BIT);glDisable(GL_BLEND);}
        field.fieldUpdate();
        if(showField){field.fieldDraw();}
        if(paused){
            if(showPart){if(!alpha){particleDraw(drawMode);}}
            if(showField == true){field.fieldDraw();}}
        if(!paused){
            if(alpha){glEnable(GL_BLEND);}
            if(showPart){particleUpdate();particleDraw(drawMode);}}
    }
    if(!simulating){
        glDisable(GL_BLEND);
        glClear(GL_COLOR_BUFFER_BIT);
        field.fieldUpdate();
        if(showField == true){field.fieldDraw();}
        if(showPart){particleDraw(drawMode);}
        if(showField){field.fieldDraw();}
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGLUT_NewFrame(); 
        GUI();
        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }
}

void timer(int v) {
        glutPostRedisplay();
        glutTimerFunc(1000/60, timer, v);
}

int main(int argc, char **argv){
    particlesNum = getResolution();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("Perlin Flow Field");
    glutFullScreen();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glewInit();
    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    glutSpecialFunc(processSpecialKeys);
    glutMainLoop(); 

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}