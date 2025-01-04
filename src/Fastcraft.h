#pragma once
#include "utils/OpenGLHeaders.h"
#include "renderer/Textures.h"


#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstring>


#include "Timer.h"
#include "level/LevelRenderer.h"
#include "gui/Font.h"
#include "Player.h"
#include "particle/ParticleEngine.h"
#include "utils/Mouse.h"
#include "utils/Keyboard.h"
#include "utils/Controller.h"
#include "character/Cube.h"
#include "character/Zombie.h"





class Fastcraft
{
public:
    int width;
    int height;

    bool appletMode;
    bool pause;

    Fastcraft(int width, int height, bool fullscreen);
    ~Fastcraft();
    void destroy();
    void run();

private:
    static void reportGLError(char *var1);
    char *errorString(GLenum glError);

    void grabMouse(GLFWwindow *window);
    void releaseMouse(GLFWwindow *window);

    void tick(GLFWwindow *window);

    void focusPlayerCamera(float var1);

    void render(float deltaTime, GLFWwindow *window);

    void setupFog(int mode);

    float *getFogColor(int mode);
    float *getBuffer(float var1, float var2, float var3, float var4);

    void init(GLFWwindow **window);

    void stop();
    void drawGui();
    void setupCamera(float timer);

    void toggleFullscreen(GLFWwindow *window);
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void pickMatrix(float x, float y, float deltaX, float deltaY, const GLint *viewport);
    void perspective(float fovY, float aspect, float zNear, float zFar);

    void setupPickCamera(float timer, int widthIn, int heightIn);
    void pick(float deltaTime);

    double mouseX;
    double mouseY;
    long long millis;
    Timer timer = Timer(20.0F);
    Level *level;
    Player *player;
    int paintTexture = 1;
    int editMode;
    bool running;

    bool mouseGrabbed;
    std::string fpsString;
    NVGcontext *vg;
    Textures *textures;
    ParticleEngine *particleEngine;
    Font *font;
    LevelRenderer *levelRenderer;
    Mouse *mouse;
    Keyboard *keyboard;
    Controller *controller;
    int stickSpeed;
    std::vector<Entity *> entities;
    HitResult *hitResult;
    bool isFullscreen;

    int windowedWidth;
    int windowedHeight;
    int windowedPosX;
    int windowedPosY;
    int var2;
    float fogColor1[4];
    int var1;
    float fogColor0[4];
    float lb[16];
};