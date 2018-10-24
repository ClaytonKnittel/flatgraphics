#version 330 core

//uniform float scale;
//uniform vec2 translation;
//uniform mat2 rotation;

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

out vec4 p_color;

void main() {
    //vec2 transl = translation + (rotation * position * scale);
    //gl_Position = vec4(transl, 0.0, 1.0);
    gl_Position = vec4(position, 0.0, 1.0);
    p_color = vec4(color);
}
