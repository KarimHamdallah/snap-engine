#version 330 core

out vec4 FragColor;

in vec2 Texcoord;

uniform sampler2D u_texture;
uniform vec4 u_color;
  
void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, Texcoord).r);
    FragColor = u_color * sampled;
}