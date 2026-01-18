#pragma once
#include "def.h"
#include <GLFW/glfw3.h>

#define HEIGHT  16
#define WIDTH   16
// Zoom size, calculated by 512/HEIGHT (just when HEIGHT == WIDTH)
#define ZOOM    32

struct Screen {
    private:
    bytes framebuffer;
    GLFWwindow* window;

    public:
    Screen(byte* addr_ptr);
    void tick();
    void terminate();
};