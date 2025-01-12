#ifndef __WINDOW_MANAGER_HPP__
#define __WINDOW_MANAGER_HPP__

#include <iostream>
#include <map>
#include <string>
#include <exception>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include FT_FREETYPE_H

#include "Window.hpp"

class WindowManager
{
public:
    WindowManager();
    ~WindowManager();

    void createWindow();

private:
    struct Character
    {
        unsigned int TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        unsigned int Advance;
    };

    std::map<char, Character> Characters;
    GLuint VAO, VBO;

    void processInput(GLFWwindow *window);
    float calculateTextWidth(const std::string& text, float scale);

    GLuint compileShader(const char* source, GLenum type);
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);

    void RenderText(GLuint shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    std::vector<Window> windows;
};

#endif
