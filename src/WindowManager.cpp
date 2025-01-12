#include "WindowManager.hpp"
#include "font.hpp"

GLuint WindowManager::compileShader(const char* source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Error: " << infoLog << std::endl;
    }
    return shader;
}

GLuint WindowManager::createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader Linking Error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void WindowManager::RenderText(GLuint shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    glUseProgram(shader);
    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (char c : text) {
        Character ch = Characters[c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


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

    // Get the primary monitor to create a fullscreen window
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);

    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "Eye Saver", primaryMonitor, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    glViewport(0, 0, mode->width, mode->height);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize FreeType library.");
    }

    FT_Face face;
    if (FT_New_Memory_Face(ft, Mx437_IBM_VGA_8x16_ttf, Mx437_IBM_VGA_8x16_ttf_len, 0, &face)) {
        FT_Done_FreeType(ft);
        throw std::runtime_error("Failed to load font from memory.");
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load glyph." << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    GLuint shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    glm::mat4 projection = glm::ortho(0.0f, (float) mode->width, 0.0f, (float) mode->height);
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    const std::string text = "HELLOOOOOOOOOOoooo...";
    float scale = 1.0f;
    
    // Calculate text dimensions
    float textWidth = calculateTextWidth(text, scale);
    float textHeight = Characters['H'].Size.y * scale; // Approximate height using a representative character
    
    // Calculate centered position
    float x = (mode->width - textWidth) / 2.0f;
    float y = (mode->height + textHeight) / 2.0f; // +textHeight because OpenGL origin is bottom-left
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT);

        RenderText(shader, text, x, y, scale, glm::vec3(1.0f, 1.0f, 1.0f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

WindowManager::~WindowManager()
{
    glfwTerminate();
}

void WindowManager::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float WindowManager::calculateTextWidth(const std::string& text, float scale)
{
    float width = 0.0f;
    for (char c : text) {
        Character ch = Characters[c];
        width += (ch.Advance >> 6) * scale; // Advance is in 1/64 pixels
    }
    return width;
}
