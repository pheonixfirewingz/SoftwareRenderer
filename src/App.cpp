#include "App.h"
#include <stdio.h>
#include <stdlib.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#ifdef IS_GCC
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wdouble-promotion"
#endif
#include <glad/gl.h>
#ifdef IS_GCC
#    pragma GCC diagnostic pop
#endif
static void error_callback(int, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void resize(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(0, 0);
    glPixelZoom((float)width, (float)height);
}

App::App(int width, int height, const char *title)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(window), resize);

    glfwMakeContextCurrent(static_cast<GLFWwindow *>(window));
    gladLoaderLoadGL();
    glfwSwapInterval(1);
    resize(nullptr, width, height);
}

void App::run()
{
    {
        int width, height;
        glfwGetFramebufferSize(static_cast<GLFWwindow *>(window), &width, &height);
        init(width, height);
    }
    int last_width = 0, last_height = 0;
    while (!glfwWindowShouldClose(static_cast<GLFWwindow *>(window)))
    {
        int width, height;
        glfwGetFramebufferSize(static_cast<GLFWwindow *>(window), &width, &height);
        if (width != last_width || height != last_height)
            resizeCallback(last_width = width, last_height = height);
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2i(0, 0);
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, render(width, height));
        glFinish();
        glfwSwapBuffers(static_cast<GLFWwindow *>(window));
        glfwPollEvents();
    }
    cleanup();
}

App::~App()
{
    gladLoaderUnloadGL();
    glfwDestroyWindow(static_cast<GLFWwindow *>(window));
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

bool App::IsKeyDown(unsigned short key) const noexcept
{
    return bool(glfwGetKey(static_cast<GLFWwindow*>(window),key) == GLFW_PRESS);
}
