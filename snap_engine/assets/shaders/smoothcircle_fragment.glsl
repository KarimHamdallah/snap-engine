#version 330 core

out vec4 FragColor;

in vec2 localPos;
uniform vec4 u_color;
uniform float u_fade;
uniform float u_thickness;
  
void main()
{
    vec2 pixelpos = localPos;
	float dist = 1.0 - length(pixelpos); // get distacne between pixel pos and origin (center of screen)
	float circleAlpha = smoothstep(0.0, u_fade, dist);
	circleAlpha *= smoothstep(u_thickness + u_fade, u_thickness, dist);

	if(circleAlpha == 0.0)
	   discard;

    FragColor = u_color;
	FragColor.a *= circleAlpha;
}