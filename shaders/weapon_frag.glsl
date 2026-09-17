#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    vec4 color = texture(tex, texCoord);

    if (color.a < 0.5)
        discard;

    FragColor = color;
}
