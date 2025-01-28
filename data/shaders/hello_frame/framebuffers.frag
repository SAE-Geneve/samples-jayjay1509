#version 300 es
precision highp float;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, TexCoords);
    FragColor = vec4(color.b, color.r, color.g, color.a);
}