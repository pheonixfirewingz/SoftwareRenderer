#include "App.h"
#include <stdlib.h>
#include <stdio.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
static void error_callback(int, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void resize(GLFWwindow*, int width, int height)
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

App::App(int width, int height,const char* title)
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

    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(window), resize);

    glfwMakeContextCurrent(static_cast<GLFWwindow*>(window));
    gladLoaderLoadGL();
    glfwSwapInterval(1);
    resize(nullptr, width, height);
}

void App::run()
{
    int last_width = 0, last_height = 0;
    while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(window)))
    {
        int width, height;
        glfwGetFramebufferSize(static_cast<GLFWwindow*>(window), &width, &height);
        if (width != last_width || height != last_height)
            resizeCallback(last_width = width, last_height = height);
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2i(0, 0);
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, render(width,height));
        glFinish();
        glfwSwapBuffers(static_cast<GLFWwindow*>(window));
        glfwPollEvents();
    }
}

App::~App()
{
    gladLoaderUnloadGL();
    glfwDestroyWindow(static_cast<GLFWwindow*>(window));
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
