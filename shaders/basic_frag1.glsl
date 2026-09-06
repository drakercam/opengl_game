#version 330 core

in vec3 colour;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main() {
    fragColor = texture(tex1, texCoord) * vec4(colour, 0.8f);
    //fragColor = mix(texture(tex1, texCoord), texture(tex2, texCoord), 0.5) * vec4(colour, 0.5f);
};
