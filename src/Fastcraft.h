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
    void destroy();
    void run();

    void showLoadingScreen(std::string var1, std::string var2);

private:
    GLFWwindow *window = nullptr;

    static void reportGLError(std::string var1);

    void grabMouse(GLFWwindow *window);
    void releaseMouse(GLFWwindow *window);

    void tick();

    void focusPlayerCamera(float var1);

    void render(float deltaTime);

    void setupFog(int mode);

    float *getBuffer(float var1, float var2, float var3, float var4);

    void toggleFullscreen(GLFWwindow *window);
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void pickMatrix(float x, float y, float deltaX, float deltaY, const GLint *viewport);
    void perspective(float fovY, float aspect, float zNear, float zFar);

    double mouseX;
    double mouseY;
    long long millis;
    Timer timer = Timer(20.0F);
    Level *level;
    Player *player;
    int paintTexture = 1;
    int yMouseAxis = 1;
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