//#version 300 es
//precision highp float;
//
//in vec3 fragColor;
//in vec2 TexCoord;
//
//layout(location = 0) out vec4 outColor;
//
//uniform sampler2D ourTexture;
//
//void main() {
//    outColor = texture(ourTexture, TexCoord) ;
//}

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}