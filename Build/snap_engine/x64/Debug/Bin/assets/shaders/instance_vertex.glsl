#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in mat4 instance_model_matrix;

uniform mat4 u_projection;

out vec2 Texcoord;
out vec4 Color;

void main()
{
    gl_Position = u_projection * instance_model_matrix * vec4(aPos, 1.0);
	Texcoord = aTexcoord;
	Color = aColor;
}