#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;

uniform mat4 u_projection;
uniform mat4 u_model;

out vec2 Texcoord;

void main()
{
    gl_Position = u_projection * u_model * vec4(aPos, 1.0);
	Texcoord = aTexcoord;
}