#version 330

uniform sampler2D tex_uni;

in vec3 uv_var;
in vec3 position_var;

out vec4 color_out;

void main(void)
{
	//vec3 color = mod(position_var * 5.0, vec3(1.0));
	vec3 color = texture(tex_uni, uv_var.xy).rgb;
	color_out = vec4(color, 1.0);
}