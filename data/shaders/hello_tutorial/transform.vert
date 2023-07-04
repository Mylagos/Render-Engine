#version 310 es
precision highp float;

layout (location = 0) in vec3 aPos;
out vec3 fragColor;
out vec2 texPos;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


vec2 positions[4] = vec2[](
    vec2(0.9, 0.9),
    vec2(0.9, -0.9),
    vec2(-0.9, -0.9),
    vec2(-0.9, 0.9)
);

vec2 texpositions[4] = vec2[](
    vec2(0.0f, 0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f, 1.0f),
    vec2(1.0f, 0.0f)
);

vec3 colors[4] = vec3[](
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f)
);

void main() {
    gl_Position = transform * vec4(aPos, 1.0f);
    fragColor = colors[gl_VertexID];
    texPos = texpositions[gl_VertexID];
}
