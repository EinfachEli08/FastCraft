#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include "Textures.h"
#include "Timer.h"
#include "level/LevelRenderer.h"
#include "Player.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global variables
double mouseX = 0.0;
double mouseY = 0.0;

long long millis = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
Timer timer(60.0F);
int fps = 0;

// Global variables
Level *level;
Player *player;
LevelRenderer *levelRenderer;

// Fog color and configuration
float fogColor[4] = {0.5f, 0.8f, 1.0f, 1.0f};

void perspective(float fovY, float aspect, float zNear, float zFar)
{
    float f = 1.0f / tanf(fovY * 0.5f * (float)M_PI / 180.0f);
    float depth = zFar - zNear;

    float matrix[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, -(zFar + zNear) / depth, -1,
        0, 0, -(2 * zNear * zFar) / depth, 0};

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrix);
}

// Callback for cursor position
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    mouseX = xpos;
    mouseY = ypos;
}

// Move camera to the player
void moveCameraToPlayer(float timer)
{
    glTranslatef(0.0f, 0.0f, -0.3f);
    glRotatef(player->xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(player->yRot, 0.0f, 1.0f, 0.0f);
    float x = player->xo + (player->x - player->xo) * timer;
    float y = player->yo + (player->y - player->yo) * timer;
    float z = player->zo + (player->z - player->zo) * timer;
    glTranslatef(-x, -y, -z);
}

// Set up the camera
void setupCamera(float timer)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(70.0f, 800.0f / 600.0f, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(timer);
}

// Render function
void render(float deltaTime, GLFWwindow *window)
{
    player->turn(mouseX, mouseY);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    setupCamera(deltaTime);

    // Enable fog
    //glEnable(GL_FOG);
    //glFogi(GL_FOG_MODE, GL_EXP);
    //glFogf(GL_FOG_DENSITY, 0.2f);
    //glFogfv(GL_FOG_COLOR, fogColor);

    // Render level
    levelRenderer->render(player, 0);
    levelRenderer->render(player, 1);

    // Disable fog and texture
   // glDisable(GL_FOG);
    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(window);
}

// Initialize GLFW and OpenGL
int initGfx(GLFWwindow **window)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    *window = glfwCreateWindow(800, 600, "FastCraft Game", nullptr, nullptr);
    if (*window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(*window);
    glfwSwapInterval(1);

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set up cursor callback
    glfwSetCursorPosCallback(*window, cursor_position_callback);

    return 0;
}

static void init()
{
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5F, 0.8F, 1.0F, 0.0F);
    glClearDepth(1.0D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    level = new Level();
    player = new Player(level);
    levelRenderer = new LevelRenderer(level);
}
void tick(){
    player->tick();
}

// Main function
int main()
{
    GLFWwindow *window = nullptr;

    if (initGfx(&window) < 0)
    {
        return -1;
    }

    init();

    while (!glfwWindowShouldClose(window))
    {
        timer.advanceTime();
        glfwPollEvents();

        for (int var4 = 0; var4 < timer.ticks; ++var4)
        {
            tick();
        }

        // Render the scene
        render(timer.a, window);

        // FPS tracking
        ++fps;
        long long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time >= millis + 1000)
        {
            std::cout << fps << " fps, Mouse: (" << mouseX << ", " << mouseY << ")" << std::endl;
            millis += 1000;
            fps = 0;
        }
    }

    // Clean up and terminate
    glfwTerminate();
    return 0;
}
