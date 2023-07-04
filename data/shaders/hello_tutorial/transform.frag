#version 310 es
precision mediump float;

in vec3 fragColor;
in vec2 texPos;

out vec4 outColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	outColor = mix(texture(texture1, texPos), texture(texture2, texPos), 0.5);
}