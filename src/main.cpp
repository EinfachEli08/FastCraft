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

int screenWidth = 960;
int screenHeight = 540;

double mouseX = 0.0;
double mouseY = 0.0;

long long millis = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
Timer timer(60.0F);
int fps = 0;

Level *level;
Player *player;
LevelRenderer *levelRenderer;
Mouse *mouse;

HitResult *hitResult;

int var1 = 920330;
float fogColor[4] = {(var1 >> 16 & 255) / 255.0F, (var1 >> 8 & 255) / 255.0F, (var1 & 255) / 255.0F, 1.0f};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, screenWidth, screenHeight);
}

void pickMatrix(float x, float y, float deltaX, float deltaY, const GLint *viewport)
{
    if (deltaX <= 0.0f || deltaY <= 0.0f)
    {
        return;
    }

    glTranslatef(
        (viewport[2] - 2.0f * (x - viewport[0])) / deltaX,
        (viewport[3] - 2.0f * (y - viewport[1])) / deltaY,
        0.0f);

    glScalef(
        static_cast<float>(viewport[2]) / deltaX,
        static_cast<float>(viewport[3]) / deltaY,
        1.0f);
}

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

void setupPickCamera(float timer, int widthIn, int heightIn)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    pickMatrix(widthIn, heightIn, 5.0f, 5.0f, viewport);

    perspective(70.0f, screenWidth / screenHeight, 0.05f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(timer);
}

void pick(float deltaTime)
{
    // Ensure selectBuffer is adequately sized
    std::vector<GLuint> selectBuffer(512);
    glSelectBuffer(selectBuffer.size(), selectBuffer.data());
    glRenderMode(GL_SELECT);

    setupPickCamera(deltaTime, screenWidth / 2, screenHeight / 2);

    // Perform the picking operation using the levelRenderer
    if (levelRenderer)
    {
        levelRenderer->pick(player); // Ensure levelRenderer is initialized properly
    }

    // Process hits from OpenGL's selection buffer
    GLint hits = glRenderMode(GL_RENDER);
    size_t bufferPos = 0;

    GLuint closestDepth = std::numeric_limits<GLuint>::max();
    int hitCount = 0;
    std::array<int, 10> pos = {0};

    for (int i = 0; i < hits; ++i)
    {
        GLuint names = selectBuffer[bufferPos++];
        GLuint minDepth = selectBuffer[bufferPos++];
        bufferPos++; // Skip maxDepth

        if (minDepth >= closestDepth && i != 0)
        {
            bufferPos += names; // Skip over the names for this hit
        }
        else
        {
            closestDepth = minDepth;
            hitCount = names;

            for (GLuint j = 0; j < names; ++j)
            {
                if (j < pos.size())
                {
                    pos[j] = selectBuffer[bufferPos++];
                }
            }
        }
    }

    // Update the hit result based on the closest hit
    if (hitCount > 0)
    {
        hitResult = new HitResult(pos[0], pos[1], pos[2], pos[3], pos[4]);
    }
    else
    {
        hitResult = nullptr;
    }
}

// Set up the camera
void setupCamera(float timer)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluPerspective(70.0F, 800 / 600, 0.05F, 1000.0F)
    perspective(70.0f, (screenWidth + 0.0F) / (screenHeight + 0.0F), 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(timer);
}

// Render function
void render(float deltaTime, GLFWwindow *window)
{
    player->turn(mouse->getDX(), mouse->getDY());
    pick(deltaTime);

    if (mouse->getEventButton() == 1 && mouse->getEventButtonState() && hitResult != nullptr)
    {
        level->setTile(hitResult->x, hitResult->y, hitResult->z, 0);
    }
    if (mouse->getEventButton() == 0 && mouse->getEventButtonState() && hitResult != nullptr)
    {
        int x = hitResult->x;
        int y = hitResult->y;
        int z = hitResult->z;
        if (hitResult->f == 0)
        {
            --y;
        }

        if (hitResult->f == 1)
        {
            ++y;
        }

        if (hitResult->f == 2)
        {
            --z;
        }

        if (hitResult->f == 3)
        {
            ++z;
        }

        if (hitResult->f == 4)
        {
            --x;
        }

        if (hitResult->f == 5)
        {
            ++x;
        }

        level->setTile(x, y, z, 1);
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS){
        level->save();
    }

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

    if (hitResult != nullptr)
    {
        levelRenderer->renderHit(*hitResult);
    }

    glDisable(GL_FOG);
    glfwSwapBuffers(window);
}

// Initialize GLFW and OpenGL
int init(GLFWwindow **window)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor)
    {
        std::cerr << "Failed to get primary monitor!" << std::endl;
        return -1;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (!mode)
    {
        std::cerr << "Failed to get video mode!" << std::endl;
        return -1;
    }

    int windowPosX = (mode->width - screenWidth) / 2;
    int windowPosY = (mode->height - screenHeight) / 2;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    *window = glfwCreateWindow(screenWidth, screenHeight, "FastCraft Game", nullptr, nullptr);
    if (*window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(*window, windowPosX, windowPosY);

    glfwMakeContextCurrent(*window);

    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(0);

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

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5F, 0.8F, 1.0F, 0.0F);
    glClearDepth(1.0D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    // TODO: Player crash when depth is smaller than width and height. noftalls dima fragen
    level = new Level(64, 64, 64);
    // TODO: Crash without reason fix when depth is smaller
    player = new Player(level);
    levelRenderer = new LevelRenderer(level);
    mouse = new Mouse();

    return 0;
}

void tick()
{
    player->tick();
}

void destroy(){
    level->save();
    glfwTerminate();
}
// Main function
int main()
{
    double xpos, ypos;
    GLFWwindow *window = nullptr;

    if (init(&window) < 0)
    {
        return -1;
    }

    while (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE) == GLFW_PRESS && !glfwWindowShouldClose(window))
    {

        timer.advanceTime();

        for (int i = 0; i < timer.ticks; ++i)
        {
            tick();
        }

        render(timer.a, window);

        ++fps;
        long long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time >= millis + 1000)
        {
            std::cout << fps << " fps" << std::endl;
            millis += 1000;
            fps = 0;
        }

        glfwGetCursorPos(window, &xpos, &ypos);
        mouse->update(xpos, -ypos);

        glfwPollEvents();
    }

    // Clean up and terminate
    destroy();

    return 0;
}
