#version 310 es
precision mediump float;

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 texPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
   FragColor = mix(texture(texture1, texPos), texture(texture1, texPos), 0.2);
}