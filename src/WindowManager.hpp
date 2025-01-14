#ifndef __WINDOW_MANAGER_HPP__
#define __WINDOW_MANAGER_HPP__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>

#include "Window.hpp"

class WindowManager
{
public:
    WindowManager();
    ~WindowManager();

    // given display id as input
    void createWindow();
    void createWindow(GLFWmonitor *monitor);
    // func for getting displays

private:
    std::vector<Window> windows;
};

#endif
