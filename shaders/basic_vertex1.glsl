#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 colour;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    colour = vec3(aPos.x + 0.25, aPos.x + aPos.y + 0.25, aPos.x + 0.5);
    texCoord = aTexCoord;
};
