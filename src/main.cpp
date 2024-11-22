#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include "Textures.h"
#include "Timer.h"
#include "level/LevelRenderer.h"
#include "Player.h"
#include "utils/Mouse.h"

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
// Create a Mouse instance
Mouse *mouse;

// Fog color and configuration
int var1 = 920330;
float fogColor[4] = {(var1 >> 16 & 255) / 255.0F, (var1 >> 8 & 255) / 255.0F, (var1 & 255) / 255.0F, 1.0f};

void perspective(float fovY, float aspect, float zNear, float zFar)
{
    float radians = fovY / 2.0f * 3.1415927f / 180.0f; 
    float deltaZ = zFar - zNear;
    float sine = std::sin(radians);

    if (deltaZ != 0.0f && sine != 0.0f && aspect != 0.0f)
    {
        float cotangent = std::cos(radians) / sine; 


        float matrix[16] = {0};
        matrix[0] = cotangent / aspect;     
        matrix[5] = cotangent;                    
        matrix[10] = -(zFar + zNear) / deltaZ;      
        matrix[11] = -1.0f;                         
        matrix[14] = -2.0f * zNear * zFar / deltaZ; 
        matrix[15] = 0.0f;                        

       
        glMultMatrixf(matrix);
    }
}

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
    // gluPerspective(70.0F, 800 / 600, 0.05F, 1000.0F)
    perspective(70.0f, 800.0F / 600.0F, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(timer);
}

// Render function
void render(float deltaTime, GLFWwindow *window)
{
    player->turn(mouse->getDX(), mouse->getDY());
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    setupCamera(deltaTime);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.2f);
    glFogfv(GL_FOG_COLOR, fogColor);

    // Render level
    glDisable(GL_FOG);
    levelRenderer->render(player, 0);
    glEnable(GL_FOG);
    levelRenderer->render(player, 1);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
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

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLuint texture;
    try
    {
        texture = Textures::loadTexture("assets/terrain.png", GL_NEAREST);
        Chunk::init(texture);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

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
    mouse = new Mouse();
}
void tick()
{
    player->tick();
}

// Main function
int main()
{
    double xpos, ypos;
    GLFWwindow *window = nullptr;

    if (initGfx(&window) < 0)
    {
        return -1;
    }

    init();

    while (!glfwWindowShouldClose(window))
    {
        timer.advanceTime();
        

        for (int i = 0; i < timer.ticks; ++i)
        {
            tick();
        }

        render(timer.a, window);

       
        glfwGetCursorPos(window, &xpos, &ypos);
        mouse->update(xpos,-ypos);

        //std::cout << " Mouse : (x : " << mouse->getX() << ", y : " << mouse->getY() << ", dx : " << mouse->getDX() << ", dy : " << mouse->getDY() << ")" << std::endl;

        ++ fps;

        long long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time >= millis + 1000)
        {
            std::cout << fps << " fps" << std::endl;
            millis += 1000;
            fps = 0;
        }

        glfwPollEvents();
    }

    // Clean up and terminate
    glfwTerminate();
    return 0;
}
