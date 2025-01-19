#include "Fastcraft.h"
#include "nano/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nano/nanovg_gl.h"

Fastcraft::Fastcraft(int width, int height, bool fullscreen)
{
    this->width = width;
    this->height = height;
    this->isFullscreen = fullscreen;
    this->textures = new Textures();
}

void Fastcraft::init(GLFWwindow **window)
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

    int windowPosX = (mode->width - this->width) / 2;
    int windowPosY = (mode->height - this->height) / 2;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    *window = glfwCreateWindow(this->width, this->height, "FastCraft Game", nullptr, nullptr);
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

    this->vg = nvgCreateGL3(NVG_STENCIL_STROKES);
    if (this->vg == nullptr)
    {
        std::cerr << "Could not initialize NanoVG" << std::endl;
        return;
    }

    this->mouse = new Mouse();
    this->keyboard = new Keyboard();

    this->checkGlError("Pre startup");

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

    this->checkGlError("Startup");

    this->level = new Level(256, 256, 64);
    this->levelRenderer = new LevelRenderer(this->level, this->textures);
    this->player = new Player(this->level);
    this->particleEngine = new ParticleEngine(this->level);
    this->font = new Font(this->vg, "assets/MinecraftRegular.ttf");

    for (int i = 0; i < 10; ++i)
    {
        auto *zombie = new Zombie(this->level, this->textures, 128.0F, 0.0F, 128.0F);
        zombie->resetPos();
        entities.push_back(zombie);
    }

    if (!appletMode)
    {
        this->grabMouse(*window);
    }

    this->checkGlError
("Post startup");
}

void Fastcraft::checkGlError
(char *var1)
{
    int var2 = glGetError();
    if (var2 != 0)
    {
        const char *errorMessage;
        switch (var2)
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

        std::cout << "########## GL ERROR ##########" << std::endl;
        std::cout << "@ " << var1 << std::endl;
        std::cout << var2 << ": " << errorMessage << std::endl;
        exit(0);
    }
}

void Fastcraft::destroy()
{
    try
    {
        this->level->save();
    }
    catch (const std::exception &e)
    {
    }

    nvgDeleteGL3(vg);
    glfwTerminate();
}

void Fastcraft::run()
{
    this->running = true;

    double xpos, ypos;
    GLFWwindow *window = nullptr;
    try
    {
        this->init(&window);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to start Minecraft " << e.what() << '\n';
        exit(0);
    }

    long long millis = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    int fps = 0;

    try
    {
        using clock = std::chrono::high_resolution_clock;

        auto start_time = clock::now();
        auto last_time = clock::now();

        while (this->running)
        {
            if (this->pause)
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
                // controller->update();

                if (keyboard->isKeyPressed(GLFW_KEY_F11)) // || controller->isButtonPressed(6)
                {
                    toggleFullscreen(window);
                }

                this->timer.advanceTime();

                for (int i = 0; i < timer.ticks; ++i)
                {
                    this->tick(window);
                }

                this->checkGlError("Pre render");
                this->render(timer.a, window);
                this->checkGlError("Post render");
                ++fps;

                auto now = clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

                fps++;

                if (elapsed >= 1000)
                {
                    this->fpsString = std::to_string(fps) + " fps, " + std::to_string(Chunk::updates) + " chunk updates";
                    Chunk::updates = 0;
                    start_time = now;
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

    this->destroy();
}

void Fastcraft::stop()
{
    this->running = false;
}

void Fastcraft::grabMouse(GLFWwindow *window)
{
    if(!this->mouseGrabbed) {
        this->mouseGrabbed = true;
        if(this->appletMode) {
            try {
                mouse->setCursorPosition(this->width / 2, this->height / 2);
            } catch (const std::exception &e)
            {
                std::cerr << "Error grabbing mouse: " << e.what() << std::endl;
            }
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            std::cout << "Mouse grabbed successfully.\n";
        }
    }
}

void Fastcraft::releaseMouse(GLFWwindow *window)
{
    if(this->mouseGrabbed) {
        this->mouseGrabbed = false;
        if(!this->appletMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

    }
}

void Fastcraft::handleMouseClick()
{
    if(this->editMode == 0) {
        if (this->hitResult != nullptr) {
            Tile *tile = Tile::tiles[this->level->getTile(this->hitResult->x, this->hitResult->y, this->hitResult->z)];
            bool var4 = this->level->setTile(this->hitResult->x, this->hitResult->y, this->hitResult->z, 0);
            if (tile != nullptr && var4) {
                tile->destroy(this->level, this->hitResult->x, this->hitResult->y, this->hitResult->z, *this->particleEngine);
            }
        }
    } else if(this->hitResult != nullptr) {
        int x = this->hitResult->x;
        int y = this->hitResult->y;
        int z = this->hitResult->z;
        if (this->hitResult->f == 0) {
            x--;
        }
        if (this->hitResult->f == 1) {
            x++;
        }
        if (this->hitResult->f == 2) {
            y--;
        }
        if (this->hitResult->f == 3) {
            y++;
        }
        if (this->hitResult->f == 4) {
            z--;
        }
        if (this->hitResult->f == 5) {
            z++;
        }

        AABB *aabb = Tile::tiles[this->paintTexture]->getTileAABB(x, y, z);
        if (aabb == nullptr || this->isFree(aabb)){
            this->level->setTile(x, y, z, this->paintTexture);
        }
    }
}

void Fastcraft::tick(GLFWwindow *window)
{
    while (mouse->next())
    {
        if (!this->mouseGrabbed && mouse->getEventButtonState())
        {
            this->grabMouse(window);
        }
        else
        {
            if (mouse->getEventButton() == GLFW_MOUSE_BUTTON_LEFT && mouse->getEventButtonState() == GLFW_PRESS)
            {
                this->handleMouseClick();
            }

            if (mouse->getEventButton() == GLFW_MOUSE_BUTTON_RIGHT && mouse->getEventButtonState() == GLFW_PRESS)
            {
                this->editMode = (this->editMode + 1) % 2;
            }
        }
    }

    while (keyboard->next())
    {
        if (keyboard->getEventKeyState())
        {
            if (keyboard->getEventKey() == GLFW_KEY_ESCAPE && (appletMode || !isFullscreen))
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
            if (keyboard->isKeyPressed(GLFW_KEY_5))
            {
                paintTexture = 6;
            }

            if (keyboard->isKeyPressed(GLFW_KEY_Y))
            {
                this->yMouseAxis *= -1;
            }

            if (keyboard->isKeyPressed(GLFW_KEY_G))
            {
                entities.push_back(new Zombie(level, textures, player->x, player->y, player->z));
            }
        }
    }

    for (int i = 0; i < this->entities.size(); ++i)
    {
        this->entities[i]->tick();
        if (this->entities[i]->removed)
        {
            this->entities.erase(this->entities.begin() + i);
        }
    }

    player->tick(controller, 0.125);
    particleEngine->tick();
}

bool Fastcraft::isFree(AABB *aabb)
{
    if (this->player->bb.intersects(*aabb))
    {
        return false;
    }
    else
    {
        for (int var2 = 0; var2 < this->entities.size(); ++var2)
        {
            if ((this->entities[var2])->bb.intersects(*aabb))
            {
                return false;
            }
        }

        return true;
    }
}

void Fastcraft::moveCameraToPlayer(float var1)
{
    glTranslatef(0.0F, 0.0F, -0.3F);
    glRotatef(player->xRot, 1.0F, 0.0F, 0.0F);
    glRotatef(player->yRot, 0.0F, 1.0F, 0.0F);
    float var2 = player->xo + (player->x - player->xo) * var1;
    float var3 = player->yo + (player->y - player->yo) * var1;
    float var4 = player->zo + (player->z - player->zo) * var1;
    glTranslatef(-var2, -var3, -var4);
}

void Fastcraft::setupCamera(float timer)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(70.0f, (width + 0.0F) / (height + 0.0F), 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(timer);
}

void Fastcraft::setupPickCamera(float timer, int widthIn, int heightIn)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    pickMatrix(widthIn, heightIn, 5.0f, 5.0f, viewport);
    perspective(70.0f, width / height, 0.05f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(timer);
}

void Fastcraft::pick(float deltaTime)
{
    std::vector<GLuint> selectBuffer(512);
    glSelectBuffer(selectBuffer.size(), selectBuffer.data());
    glRenderMode(GL_SELECT);

    setupPickCamera(deltaTime, width / 2, height / 2);

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

void Fastcraft::render(float deltaTime, GLFWwindow *window)
{
    // this->releaseMouse(window);
    // player->turn(mouse->getDX(), mouse->getDY());
    if (this->mouseGrabbed)
    {
        float var2 = 0.0F;
        float var3 = 0.0F;
        var2 = (float)mouse->getDX();
        var3 = (float)mouse->getDY();
        if (this->appletMode)
        {
            mouse->poll();
            var2 = (float)(mouse->getX() - this->width / 2);
            var3 = (float)(mouse->getY() - this->height / 2);
            mouse->setCursorPosition(this->width / 2, this->height / 2);
        }

        this->player->turn(var2, var3 * (float)this->yMouseAxis);
    }
    // TODO: Implement controller support again
    this->checkGlError
("Set Viewport");
    this->pick(deltaTime);
    this->checkGlError
("Picked");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    this->setupCamera(deltaTime);
    this->checkGlError
("Set up camera");
    glEnable(GL_CULL_FACE);
    Frustum frustum = Frustum::getInstance();
    this->levelRenderer->updateDirtyChunks(player);
    this->checkGlError
("Update Chunks");
    this->setupFog(0);
    glEnable(GL_FOG);
    this->levelRenderer->render(player, 0);
    this->checkGlError
("Rendered level");

    for (int index = 0; index < entities.size(); ++index)
    {
        Entity &entity = *entities[index];
        if (entity.isLit() && frustum.isVisible(&(entity.bb)))
        {
            entity.render(deltaTime);
        }
    }

this->checkGlError
("Rendered entities");
    particleEngine->render(player, deltaTime, 0);
this->checkGlError
("Rendered particles");
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
this->checkGlError
("Rendered rest");
    if (hitResult != nullptr)
    {
        glDisable(GL_ALPHA_TEST);
        levelRenderer->renderHit(*hitResult, editMode, paintTexture);
        glEnable(GL_ALPHA_TEST);
    }
this->checkGlError
("Rendered hit");
    drawGui(deltaTime);
this->checkGlError
("Rendered gui");
    glfwSwapBuffers(window);
}

void Fastcraft::drawGui(float deltaTime)
{
    int sw = this->width * 240 / this->height;
    int sh = this->height * 240 / this->height;
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0D, (double)sw, (double)sh, 0.0D, 100.0D, 300.0D);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0F, 0.0F, -200.0F);

this->checkGlError
("GUI: Init");

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
    Tile::tiles[this->paintTexture]->render(tess, this->level, 0, -2, 0, 0);
    tess.flush();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

this->checkGlError
("GUI: Draw selected");

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

this->checkGlError
("GUI: Draw crosshair");

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    // NanoVG rendering
    nvgBeginFrame(vg, this->width, this->height, 1.0f);

    font->drawShadow("FastCraft Engine Classic", 4.0f, 4.0f, 16.0f, nvgRGBA(255, 255, 255, 255));
    font->drawShadow(fpsString, 4.0f, 24.0f, 16.0f, nvgRGBA(255, 255, 255, 255));
    font->drawShadow("paintTexture: " + std::to_string(paintTexture), 4.0f, 44.0f, 16.0f, nvgRGBA(255, 255, 255, 255));

this->checkGlError
("GUI: Draw text");

    nvgEndFrame(vg);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopClientAttrib();
    glPopAttrib();

this->checkGlError
("GUI: NVG font");
}

void Fastcraft::setupFog(int mode)
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

float *Fastcraft::getBuffer(float var1, float var2, float var3, float var4)
{
    lb[0] = var1;
    lb[1] = var2;
    lb[2] = var3;
    lb[3] = var4;
    return lb;
}

/* Extra classes */
void Fastcraft::toggleFullscreen(GLFWwindow *window)
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

void Fastcraft::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    width = width;
    height = height;
    glViewport(0, 0, width, height);
}

void Fastcraft::pickMatrix(float x, float y, float deltaX, float deltaY, const GLint *viewport)
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

void Fastcraft::perspective(float fovY, float aspect, float zNear, float zFar)
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
