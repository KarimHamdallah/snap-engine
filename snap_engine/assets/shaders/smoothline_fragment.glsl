#version 330 core

out vec4 FragColor;

in vec2 localPos;
uniform vec4 u_color;
uniform vec2 u_resolution;
uniform vec2 u_startPoint;
uniform vec2 u_endPoint;
uniform float u_thickness;


void main()
{
    FragColor = vec4(0.0);
    vec2 pixelpos = localPos * u_resolution;
	vec2 p1 = u_startPoint;
	vec2 p2 = u_endPoint;

	// line vector
	vec2 p12 = p2 - p1;
	vec2 p13 = pixelpos - p1;

	// TODO:: points relative to scale so you can get them in local pos

	float d = dot(p12, p13) / length(p12); // = length(p13) * cos(theta)
	vec2 p4 = p1 + normalize(p12) * d;

	if(length(p4 - pixelpos) < u_thickness && length(p4 - p1) <= length(p12) && length(p4 - p2) <= length(p12))
	{
	  FragColor = u_color;
	}
	else
	{
	   discard;
	}
}