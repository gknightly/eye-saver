#ifndef __SHADERS_HPP__
#define __SHADERS_HPP__

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
    out vec2 TexCoords;

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;

    uniform sampler2D text;
    uniform vec3 textColor;

    void main()
    {
        float alpha = texture(text, TexCoords).r;
        color = vec4(1.0) * alpha;
    }
)";

#endif
