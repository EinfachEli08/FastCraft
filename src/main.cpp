#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include "Textures.h"
#include "Timer.h"

int main()
{
    long long millis = std::chrono::system_clock::now().time_since_epoch().count() / 1000000; // Current time in milliseconds
    Timer timer(60.0F);
    int fps = 0;

    glfwInit();
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "FastCraft Game", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
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
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        timer.advanceTime();
        glfwPollEvents();

        glClearColor(0.5f, 0.8f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);

        Textures::bind(texture);

        float squareSize = 0.5f;

        glBegin(GL_TRIANGLES);

        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-squareSize, -squareSize);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(squareSize, -squareSize);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-squareSize, squareSize);

        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(squareSize, -squareSize);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(squareSize, squareSize);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-squareSize, squareSize);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glfwSwapBuffers(window);

        ++fps;

        long long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time >= millis + 1000)
        {
            std::cout << fps << " fps, " << std::endl;

            millis += 1000;
            fps = 0;
        }
    }

    glfwTerminate();
    return 0;
}
