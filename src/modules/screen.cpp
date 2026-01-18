#include "screen.h"
#include "errors.h"

Screen::Screen(byte* addr_ptr) 
{
    framebuffer = addr_ptr;

    if (!glfwInit()) 
    {
        glfwTerminate();
        raise(Errors::SIGABRT); // Abnormal termination
    }

    window = glfwCreateWindow(
        WIDTH * ZOOM,
        HEIGHT * ZOOM,
        "Neodymium vScreen",
        NULL,
        NULL
    );

    if (!window) 
    {
        glfwTerminate();
        raise(Errors::SIGABRT);
    }

    glfwMakeContextCurrent(window);
};

void Screen::tick()
{
    if(glfwWindowShouldClose(window))
    { 
        glfwTerminate(); 
        raise(Errors::SIGKILL);
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glPixelZoom(ZOOM, ZOOM);
    glRasterPos2i(0, 0);

    glDrawPixels(
        WIDTH,
        HEIGHT,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        framebuffer
    );

    glfwSwapBuffers(window);
    glfwPollEvents();
};

void Screen::terminate()
{
    glfwTerminate();
}