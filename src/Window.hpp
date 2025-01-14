#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <exception>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include FT_FREETYPE_H

#include "Message.hpp"

class Window
{
public:
    Window(GLFWmonitor *monitor);
    ~Window();

private:
    std::vector<Message> messages;

    struct Character
    {
        unsigned int TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        unsigned int Advance;
    };

    GLFWmonitor *monitor;
    GLFWwindow *window;

    std::map<char, Character> Characters;
    GLuint VAO, VBO;

    void processInput();

    float calculateTextWidth(const std::string& text, float scale);

    GLuint compileShader(const char* source, GLenum type);
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);

    void RenderText(GLuint shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif
