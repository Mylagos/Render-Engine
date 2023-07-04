#version 310 es
precision highp float;

layout (location = 0) in vec3 aPos;
out vec3 fragColor;

vec2 positions[4] = vec2[](
    vec2(0.9, 0.9),
    vec2(0.9, -0.9),
    vec2(-0.9, -0.9),
    vec2(-0.9, 0.9)
);

vec3 colors[4] = vec3[](
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f)
);

void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    fragColor = colors[gl_VertexID];
}
