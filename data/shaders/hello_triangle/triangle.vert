#version 300 es
precision highp float;

out vec3 fragColor;
out vec2 TexCoord;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0)
);

vec2 texCoords[3] = vec2[](
    vec2(0.0, 1.0),  // lower-left corner
    vec2(1.0, 1.0),  // lower-right corner
    vec2(0.5, 0.0)   // bottom-center corner
);

void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    fragColor = colors[gl_VertexID];
    TexCoord = texCoords[gl_VertexID];
}
