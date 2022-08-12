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
	vec3 color = vec3(smoothstep(0.0, u_fade, dist));
	color *= vec3(smoothstep(u_thickness + u_fade, u_thickness, dist));

	if(color == vec3(0.0))
	  discard;  // ignore pixel

    FragColor = vec4(color, 1.0);
	FragColor.rgb *= u_color;
}