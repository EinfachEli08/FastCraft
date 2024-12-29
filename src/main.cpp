#include "utils/OpenGLHeaders.h"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstring>
#include "renderer/Textures.h"
#include "Timer.h"
#include "level/LevelRenderer.h"
#include "Player.h"
#include "particle/ParticleEngine.h"
#include "utils/Mouse.h"
#include "utils/Keyboard.h"
#include "utils/Controller.h"
#include "character/Cube.h"
#include "character/Zombie.h"

int screenWidth = 854;
int screenHeight = 480;

double mouseX = 0.0;
double mouseY = 0.0;

long long millis = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
Timer timer(20.0F);

Level *level;
Player *player;
int paintTexture = 1;
int editMode = 0;

bool running = false;
bool pause = false;
bool mouseGrabbed = false;

Textures *textures;
ParticleEngine *particleEngine;
LevelRenderer *levelRenderer;
Mouse *mouse;
Keyboard *keyboard;
Controller *controller;
int stickSpeed = 9;
std::vector<Entity *> entities;
HitResult *hitResult;

bool isFullscreen = false;
bool appletMode = false;

int windowedWidth = 960, windowedHeight = 540;

int windowedPosX = 0, windowedPosY = 0;

int var2 = 920330;
float fogColor1[4] = {(var2 >> 16 & 255) / 255.0F, (var2 >> 8 & 255) / 255.0F, (var2 & 255) / 255.0F, 1.0f};
int var1 = 16710650;
float fogColor0[4] = {(var1 >> 16 & 255) / 255.0F, (var1 >> 8 & 255) / 255.0F, (var1 & 255) / 255.0F, 1.0f};
float lb[16];

float *getBuffer(float var1, float var2, float var3, float var4)
{
    lb[0] = var1;
    lb[1] = var2;
    lb[2] = var3;
    lb[3] = var4;
    return lb;
}

char *errorString(GLenum glError)
{
    const char *errorMessage;
    switch (glError)
    {
    case GL_NO_ERROR:
        errorMessage = "No error";
        break;
    case GL_INVALID_ENUM:
        errorMessage = "Invalid enum";
        break;
    case GL_INVALID_VALUE:
        errorMessage = "Invalid value";
        break;
    case GL_INVALID_OPERATION:
        errorMessage = "Invalid operation";
        break;
    case GL_STACK_OVERFLOW:
        errorMessage = "Stack overflow";
        break;
    case GL_STACK_UNDERFLOW:
        errorMessage = "Stack underflow";
        break;
    case GL_OUT_OF_MEMORY:
        errorMessage = "Out of memory";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        errorMessage = "Invalid framebuffer operation";
        break;
    default:
        errorMessage = "Unknown error";
        break;
    }

    // Allocate memory for the returned string
    char *result = new char[strlen(errorMessage) + 1];
    strcpy(result, errorMessage);

    return result; // Caller is responsible for freeing this memory with delete[]
}

void checkGlError(char *var1)
{
    int var2 = glGetError();
    if (var2 != 0)
    {
        char *var3 = errorString(var2);
        // std::cout << "########## GL ERROR ##########" << std::endl;
        // std::cout << "@ " << var1 << std::endl;
        // std::cout << var2 << ": " << var3 << std::endl;
        //  exit(0);
    }
}

void setupFog(int mode)
{
    if (mode == 0)
    {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.001F);
        glFogfv(GL_FOG_COLOR, fogColor0);
        glDisable(GL_LIGHTING);
    }
    else if (mode == 1)
    {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.06F);
        glFogfv(GL_FOG_COLOR, fogColor1);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        float ambientLight = 0.6F;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, getBuffer(ambientLight, ambientLight, ambientLight, 1.0F));
    }
}

void toggleFullscreen(GLFWwindow *window)
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    if (isFullscreen)
    {
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
    }
    else
    {
        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    isFullscreen = !isFullscreen;
    appletMode = !appletMode;
}

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

void moveCameraToPlayer(float var1)
{
    glTranslatef(0.0F, 0.0F, -0.3F);
    glRotatef(player->xRot, 1.0F, 0.0F, 0.0F);
    glRotatef(player->yRot, 0.0F, 1.0F, 0.0F);
    float var2 = player->xo + (player->x - player->xo) * var1;
    float var3 = player->yo + (player->y - player->yo) * var1;
    float var4 = player->zo + (player->z - player->zo) * var1;
    glTranslatef(-var2, -var3, -var4);
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
    std::vector<GLuint> selectBuffer(512);
    glSelectBuffer(selectBuffer.size(), selectBuffer.data());
    glRenderMode(GL_SELECT);

    setupPickCamera(deltaTime, screenWidth / 2, screenHeight / 2);

    levelRenderer->pick(player, Frustum::getInstance());

    GLint hits = glRenderMode(GL_RENDER);
    GLuint closestDepth = 0;
    int pos[10] = {0};
    int hitCount = 0;

    auto it = selectBuffer.begin();
    for (int i = 0; i < hits; ++i)
    {
        GLuint names = *it++;
        GLuint minDepth = *it++;
        it++;

        if (minDepth >= closestDepth && i != 0)
        {
            for (GLuint j = 0; j < names; ++j)
            {
                ++it;
            }
        }
        else
        {
            closestDepth = minDepth;
            hitCount = names;
            for (GLuint j = 0; j < names; ++j)
            {
                pos[j] = *it++;
            }
        }
    }

    if (hitCount > 0)
    {
        hitResult = new HitResult(pos[0], pos[1], pos[2], pos[3], pos[4]);
    }
    else
    {
        hitResult = nullptr;
    }
}

void drawGui()
{
    int sw = screenWidth * 240 / screenHeight;
    int sh = screenHeight * 240 / screenHeight;
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0D, (double)sw, (double)sh, 0.0D, 100.0D, 300.0D);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0F, 0.0F, -200.0F);
    checkGlError("GUI: Init");
    glPushMatrix();
    glTranslatef((float)(sw - 16), 16.0F, 0.0F);
    Tesselator &tess = Tesselator::getInstance();
    glScalef(16.0F, 16.0F, 16.0F);
    glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
    glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
    glTranslatef(-1.5F, 0.5F, -0.5F);
    glScalef(-1.0F, -1.0F, 1.0F);

    GLuint text;
    try
    {
        text = textures->loadTexture("assets/terrain.png", 9728);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, text);
    glEnable(GL_TEXTURE_2D);
    tess.init();
    Tile::tiles[paintTexture]->render(tess, level, 0, -2, 0, 0);
    tess.flush();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    checkGlError("GUI: Draw selected");
    // TODO: Fontrenderer using NANOVG
    checkGlError("GUI: Draw text");
    int wCenter = sw / 2;
    int hCenter = sh / 2;
    glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
    tess.init();
    tess.vertex((wCenter + 1), (hCenter - 4), 0.0F);
    tess.vertex((wCenter - 0), (hCenter - 4), 0.0F);

    tess.vertex((wCenter - 0), (hCenter + 5), 0.0F);
    tess.vertex((wCenter + 1), (hCenter + 5), 0.0F);

    tess.vertex((wCenter + 5), (hCenter - 0), 0.0F);
    tess.vertex((wCenter - 4), (hCenter - 0), 0.0F);

    tess.vertex((wCenter - 4), (hCenter + 1), 0.0F);
    tess.vertex((wCenter + 5), (hCenter + 1), 0.0F);
    tess.flush();
    checkGlError("GUI: Draw crosshair");
}

void setupCamera(float timer)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(70.0f, (screenWidth + 0.0F) / (screenHeight + 0.0F), 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(timer);
}

void grabMouse(GLFWwindow *window)
{
    try
    {
        if (!mouseGrabbed)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseGrabbed = true;
            std::cout << "Mouse grabbed successfully.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error grabbing mouse: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred while grabbing the mouse." << std::endl;
    }
}

void releaseMouse(GLFWwindow *window)
{
    try
    {
        if (mouseGrabbed)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseGrabbed = false;
            std::cout << "Mouse released successfully.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error releasing mouse: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred while releasing the mouse." << std::endl;
    }
}

void render(float deltaTime, GLFWwindow *window)
{
    if (!controller->isConnected())
    {
        player->turn(mouse->getDX(), mouse->getDY());
        pick(deltaTime);

        if (mouse->isButtonClicked(1) && hitResult != nullptr)
        {
            Tile *tile = Tile::tiles[level->getTile(hitResult->x, hitResult->y, hitResult->z)];
            bool var4 = level->setTile(hitResult->x, hitResult->y, hitResult->z, 0);
            if (tile != nullptr && var4)
            {
                tile->destroy(level, hitResult->x, hitResult->y, hitResult->z, *particleEngine);
            }
        }
        if (mouse->isButtonClicked(0) && hitResult != nullptr)
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

            level->setTile(x, y, z, paintTexture);
        }
    }
    else
    {
        player->turn(stickSpeed * controller->getAxisPosition(2), stickSpeed * -controller->getAxisPosition(3));
        pick(deltaTime);

        if (controller->isAxisButtonPressed(4) && hitResult != nullptr)
        {
            Tile *tile = Tile::tiles[level->getTile(hitResult->x, hitResult->y, hitResult->z)];
            bool var4 = level->setTile(hitResult->x, hitResult->y, hitResult->z, 0);
            if (tile != nullptr && var4)
            {
                tile->destroy(level, hitResult->x, hitResult->y, hitResult->z, *particleEngine);
            }
        }
        if (controller->isAxisButtonPressed(5) && hitResult != nullptr)
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

            level->setTile(x, y, z, paintTexture);
        }
    }
    checkGlError("Picked");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    setupCamera(deltaTime);
    checkGlError("Set up camera");
    glEnable(GL_CULL_FACE);
    Frustum frustum = Frustum::getInstance();
    levelRenderer->updateDirtyChunks(player);
    checkGlError("Update Chunks");
    setupFog(0);
    glEnable(GL_FOG);
    levelRenderer->render(player, 0);
    checkGlError("Rendered level");

    for (int index = 0; index < entities.size(); ++index)
    {
        Entity &entity = *entities[index];
        if (entity.isLit() && frustum.isVisible(&(entity.bb)))
        {
            entity.render(deltaTime); 
        }
    }

    checkGlError("Rendered entities");
    particleEngine->render(player, deltaTime, 0);
    checkGlError("Rendered particles");
    setupFog(1);
    levelRenderer->render(player, 1);

    for (int index = 0; index < entities.size(); ++index)
    {
        Entity &entity = *entities[index];
        if (!entity.isLit() && frustum.isVisible(&(entity.bb)))
        {
            entity.render(deltaTime); 
        }
    }

    particleEngine->render(player, deltaTime, 1);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    checkGlError("Rendered rest");
    if (hitResult != nullptr)
    {
        glDisable(GL_ALPHA_TEST);
        levelRenderer->renderHit(*hitResult, editMode, paintTexture);
        glEnable(GL_ALPHA_TEST);
    }
    checkGlError("Rendered hit");
    drawGui();
    checkGlError("Rendered gui");
    glfwSwapBuffers(window);
}

void init(GLFWwindow **window)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor)
    {
        std::cerr << "Failed to get primary monitor!" << std::endl;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (!mode)
    {
        std::cerr << "Failed to get video mode!" << std::endl;
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
    }

    checkGlError("Pre startup");

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5F, 0.8F, 1.0F, 0.0F);
    glClearDepth(1.0D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5F);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    checkGlError("Startup");

    textures = new Textures();
    level = new Level(256, 256, 64);
    player = new Player(level);
    levelRenderer = new LevelRenderer(level, textures);
    mouse = new Mouse();
    keyboard = new Keyboard();
    controller = new Controller(GLFW_JOYSTICK_1);
    particleEngine = new ParticleEngine(level);

    for (int i = 0; i < 10; ++i)
    {
        Zombie *zombie = new Zombie(level, textures, 128.0F, 0.0F, 128.0F);
        zombie->resetPos();
        entities.push_back(zombie);
        std::cout << "Added Zombie + " << i + 1 << std::endl;
    }

    if (!appletMode)
    {
        grabMouse(*window);
    }

    checkGlError("Post startup");
}

void tick(GLFWwindow *window)
{

    if (keyboard->getEventKey() == keyboard->isKeyPressed(GLFW_KEY_ESCAPE) && (appletMode || !isFullscreen))
    {
        releaseMouse(window);
    }
    if (keyboard->isKeyPressed(GLFW_KEY_ENTER))
    {
        level->save();
    }

    if (keyboard->isKeyPressed(GLFW_KEY_1))
    {
        paintTexture = 1;
    }
    if (keyboard->isKeyPressed(GLFW_KEY_2))
    {
        paintTexture = 3;
    }
    if (keyboard->isKeyPressed(GLFW_KEY_3))
    {
        paintTexture = 4;
    }
    if (keyboard->isKeyPressed(GLFW_KEY_4))
    {
        paintTexture = 5;
    }
    if (keyboard->isKeyPressed(GLFW_KEY_4))
    {
        paintTexture = 6;
    }

    if (keyboard->isKeyPressed(GLFW_KEY_G))
    {
        entities.push_back(new Zombie(level, textures, player->x, player->y, player->z));
    }

    for (int i = 0; i < entities.size(); ++i)
    {
        entities[i]->tick();
    }

    player->tick(controller, 0.125);
}

void destroy()
{
    try
    {
        level->save();
    }
    catch (const std::exception &e)
    {
    }

    glfwTerminate();
}

void stop()
{
    running = false;
}

void run()
{
    running = true;

    double xpos, ypos;
    GLFWwindow *window = nullptr;
    try
    {
        init(&window);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to start Minecraft " << e.what() << '\n';
        exit(0);
    }

    long long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    int fps = 0;

    try
    {
        while (running)
        {
            if (pause)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            else
            {

                if (glfwWindowShouldClose(window))
                {
                    stop();
                }

                mouse->update(xpos, -ypos);
                keyboard->update();
                controller->update();

                if (keyboard->isKeyPressed(GLFW_KEY_F11) || controller->isButtonPressed(6))
                {
                    toggleFullscreen(window);
                }

                timer.advanceTime();

                for (int i = 0; i < timer.ticks; ++i)
                {
                    tick(window);
                }

                checkGlError("Pre render");
                render(timer.a, window);
                checkGlError("Post render");

                ++fps;
                if (current_time >= millis + 1000)
                {
                    std::cout << fps << " fps" << std::endl;
                    millis += 1000;
                    fps = 0;
                }

                glfwGetCursorPos(window, &xpos, &ypos);

                glfwPollEvents();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    destroy();
}

int main()
{
    appletMode = true;
    run();

    return 0;
}
