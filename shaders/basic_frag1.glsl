#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D materialTexture;

void main()
{
    FragColor = texture(materialTexture, texCoord);
}
