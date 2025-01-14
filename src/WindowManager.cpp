#include "WindowManager.hpp"

WindowManager::WindowManager()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

WindowManager::~WindowManager()
{
    glfwTerminate();
}

void WindowManager::createWindow()
{
    // Get the primary monitor to create a fullscreen window
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();

    createWindow(primaryMonitor);
}

void WindowManager::createWindow(GLFWmonitor *monitor)
{
    windows.emplace_back(monitor);
}

