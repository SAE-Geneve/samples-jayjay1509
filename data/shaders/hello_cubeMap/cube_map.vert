//#version 300 es
//precision highp float;
//
//
//layout(location = 0) in vec3 position; // Vertex position
//layout(location = 1) in vec2 texCoord; // Texture coordinate
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//out vec3 fragColor;
//out vec2 TexCoord;
//
//vec3 colors[6] = vec3[](
//vec3(1.0, 0.0, 0.0),
//vec3(0.0, 1.0, 0.0),
//vec3(0.0, 0.0, 1.0),
//vec3(0.0, 0.0, 1.0),
//vec3(0.0, 1.0, 0.0),
//vec3(1.0, 0.0, 0.0)
//);
//
//
//void main() {
//    gl_Position = projection * view * model * vec4(position, 1.0);
//    fragColor = colors[gl_VertexID % 6];
//    TexCoord = texCoord;
//}


#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}