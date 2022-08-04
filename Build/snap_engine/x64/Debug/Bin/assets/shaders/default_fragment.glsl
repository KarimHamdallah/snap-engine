#version 330 core

out vec4 FragColor;

in vec2 Texcoord;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
    FragColor = /*texture(u_texture, Texcoord) * */u_color;
}