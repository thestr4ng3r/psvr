#version 330

in vec3 color_var;
in vec3 position_var;

out vec4 color_out;

void main(void)
{
	vec3 color = mod(position_var * 5.0, vec3(1.0));
	color_out = vec4(color, 1.0);
}